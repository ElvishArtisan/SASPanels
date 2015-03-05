// usi_connect.cpp
//
// A connection driver for USI devices.
//
//   (C) Copyright 2002-2005,2015 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <usi_connect.h>


USIConnect::USIConnect(QObject *parent,const char *name)
  : QObject(parent,name)
{
  //
  // Initialize Variables
  //
  usi_ptr=0;
  usi_started=false;
  usi_input_names[0]=tr("- OFF -");

  //
  // The Socket
  //
  usi_socket=new QSocket(this,"usi_socket");
  connect(usi_socket,SIGNAL(connectionClosed()),
	  this,SLOT(connectionClosedData()));
  connect(usi_socket,SIGNAL(readyRead()),this,SLOT(readyReadData()));
  connect(usi_socket,SIGNAL(error(int)),this,SLOT(errorData(int)));
}


USIConnect::~USIConnect()
{
}


void USIConnect::connectToHost(QString hostname,Q_UINT16 hostport)
{
  usi_socket->connectToHost(hostname,hostport);
}


unsigned USIConnect::inputs() const
{
  return usi_input_names.size();
}


unsigned USIConnect::inputNumber(unsigned offset)
{
  if(offset>=usi_input_ptrs.size()) {
    return 9999;
  }
  return usi_input_ptrs[offset];
}


unsigned USIConnect::inputOffset(unsigned input)
{
  for(unsigned i=0;i<usi_input_ptrs.size();i++) {
    if(usi_input_ptrs[i]==input) {
      return i;
    }
  }
  return 9999;
}


QString USIConnect::inputName(unsigned pos,bool alphabetical)
{
  if(pos>=usi_input_ptrs.size()) {
    return QString("--------");
  }
  if(alphabetical) {
    return usi_input_names[usi_input_ptrs[pos]];
  }
  return usi_input_names[pos];
}


unsigned USIConnect::outputs() const
{
  return usi_output_names.size();
}


unsigned USIConnect::outputNumber(unsigned offset)
{
  if(offset>=usi_output_ptrs.size()) {
    return 9999;
  }
  return usi_output_ptrs[offset]+1;
}


unsigned USIConnect::outputOffset(unsigned output)
{
  unsigned out=output-1;
  for(unsigned i=0;i<usi_output_ptrs.size();i++) {
    if(usi_output_ptrs[i]==out) {
      return i;
    }
  }
  return 9999;
}


QString USIConnect::outputName(unsigned pos,bool alphabetical)
{
  if(alphabetical) {
    if(pos>=usi_output_ptrs.size()) {
      return QString("--------");
    }
    return usi_output_names[usi_output_ptrs[pos]];
  }
  if((pos-1)>=usi_output_ptrs.size()) {
    return QString("--------");
  }
  return usi_output_names[pos-1];
}


void USIConnect::queryOutput(unsigned output)
{
  QString str=QString().sprintf("%c50%04u\x0D\x0A",26,output);
  usi_socket->writeBlock(str,str.length());
}


void USIConnect::switchCrosspoint(unsigned input,unsigned output,int gain,
				  SwitchAction action)
{
  if((input>=usi_input_names.size())||(output>=usi_output_names.size())) {
    return;
  }
  QString str;
  switch(action) {
      case USIConnect::Take:
	//str=QString().sprintf("%c%03d%03d\x0D\x0A",20,input,output);
	str=QString().sprintf("%cT%04d%04d\x0D\x0A",5,input,output);
	break;

      case USIConnect::Sum:
	break;

      default:
	break;
  }
  usi_socket->writeBlock(str,str.length());
  queryOutput(output);
}


void USIConnect::connectionClosedData()
{
  printf("Connection Closed!\n");
}


void USIConnect::readyReadData()
{
  char buffer[256];
  unsigned n;

  while((n=usi_socket->readBlock(buffer,255))>0) {
    buffer[n]=0;
    for(unsigned i=0;i<n;i++) {
      if(buffer[i]==10) {  // End of line
	usi_buffer[--usi_ptr]=0;
	DispatchCommand();
	usi_ptr=0;
      }
      else {
	if(usi_ptr==USI_MAX_LENGTH) {  // Buffer overflow
	  usi_ptr=0;
	}
	usi_buffer[usi_ptr++]=buffer[i];
      }
    }
  }
}


void USIConnect::errorData(int err)
{
  switch((QSocket::Error)err) {
      case QSocket::ErrConnectionRefused:
	emit connectionStatusChanged(USIConnect::Refused);
	break;

      case QSocket::ErrHostNotFound:
	emit connectionStatusChanged(USIConnect::DnsFailed);
	break;

      default:
	emit connectionStatusChanged(USIConnect::Other);
	break;
  }
}


void USIConnect::DispatchCommand()
{
  char buffer[USI_MAX_LENGTH];
  unsigned input;
  unsigned output;
  QString label;

  // printf("DISPATCHED: %s\n",usi_buffer);

  //
  // Startup Sequence.  Get the input and output lists.  The response
  // to the ^EI command lets us know when the lists are done.
  //
  if(QString("login sucessful")==(QString(usi_buffer).lower())) {
    sprintf(buffer,"%cX9999\x0D\x0A",5);  // Request Input List
    usi_socket->writeBlock(buffer,strlen(buffer));
    sprintf(buffer,"%cY9999\x0D\x0A",5);  // Request Output List
    usi_socket->writeBlock(buffer,strlen(buffer));
    sprintf(buffer,"%cI0001\x0D\x0A",5);  // Start Finished
    usi_socket->writeBlock(buffer,strlen(buffer));
    return;
  }

  //
  // Work around the idiot 'SAS READY' prompt
  //
  if(usi_buffer[0]==27) {
    for(unsigned i=17;i<strlen(usi_buffer)+1;i++) {
      usi_buffer[i-17]=usi_buffer[i];
    }
  }

  switch(usi_buffer[0]) {
      case 21:   // Input Name [^U]
	if(strlen(usi_buffer)<13) {
	  return;
	}
	label=usi_buffer+5;
	usi_buffer[5]=0;
	if(sscanf(usi_buffer+1,"%u",&input)!=1) {
	  return;
	}
	usi_input_names[input]=label;
	if(usi_started) {
	  BubbleSort(&usi_input_names,&usi_input_ptrs);
	  emit inputListChanged();
	}
	break;

      case 22:   // Output Name [^V]
	if(strlen(usi_buffer)<13) {
	  return;
	}
	label=usi_buffer+5;
	usi_buffer[5]=0;
	if(sscanf(usi_buffer+1,"%u",&output)!=1) {
	  return;
	}
	usi_output_names[output-1]=label;
	if(usi_started) {
	  BubbleSort(&usi_output_names,&usi_output_ptrs);
	  emit outputListChanged();
	}
	break;

      case 'D':  // Initialization Complete
	BubbleSort(&usi_input_names,&usi_input_ptrs);
	BubbleSort(&usi_output_names,&usi_output_ptrs);
	usi_started=true;
	emit connectionStatusChanged(USIConnect::Connected);
	emit inputListChanged();
	emit outputListChanged();
	break;

      case 'T':  // Channel Tally
	for(unsigned i=1;i<strlen(usi_buffer);i++) {
	  if(usi_buffer[i]==':') {
	    usi_buffer[i]=0;
	    if(sscanf(usi_buffer+1,"%u",&input)!=1) {
	      return;
	    }
	    if(sscanf(usi_buffer+i+1,"%u",&output)!=1) {
	      return;
	    }
	    if((input>=usi_input_names.size())||
	       (output>=usi_output_names.size())) {
	      return;
	    }
	    std::vector<unsigned> *inputs=new std::vector<unsigned>;
	    inputs->push_back(input);
	    emit outputChanged(output,inputs);
	    delete inputs;
	    return;
	  }
	}
	return;

      case 'U':  // Channel Status Update
	if(strlen(usi_buffer)<9) {
	  return;
	}
	std::vector<unsigned> *inputs=new std::vector<unsigned>;
	for(unsigned i=strlen(usi_buffer)-4;i>=9;i-=4) {
	  if(sscanf(usi_buffer+i,"%u",&input)==1) {
	    inputs->push_back(input);
	  }
	  usi_buffer[i]=0;
	}
	usi_buffer[5]=0;
	if(sscanf(usi_buffer+1,"%u",&output)!=1) {
	  delete inputs;
	  return;
	}
	emit outputChanged(output,inputs);
	delete inputs;
  }
}


void USIConnect::BubbleSort(std::map<unsigned,QString> *names,
			    std::vector<unsigned> *ptrs)
{
  //
  // Reset Pointer Table
  //
  ptrs->clear();
  for(unsigned i=0;i<names->size();i++) {
    ptrs->push_back(i);
  }

  //
  // Sort
  //
  bool changed=true;
  while(changed) {
    changed=false;
    for(unsigned i=1;i<names->size();i++) {
      if((*names)[ptrs->at(i-1)]>(*names)[ptrs->at(i)]) {
	unsigned ptr=ptrs->at(i-1);
	ptrs->at(i-1)=ptrs->at(i);
	ptrs->at(i)=ptr;
	changed=true;
      }
    }
  }
}
