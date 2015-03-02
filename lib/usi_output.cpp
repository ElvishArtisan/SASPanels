// usi_output.cpp
//
// A USI-based single output input selector.
//
//   (C) Copyright 2002-2005 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdlib.h>

#include <qmessagebox.h>

#include <usi_output.h>


USIOutput::USIOutput(USIConfig *config,USIConnect *connect,unsigned output,
		       QWidget *parent,const char *name)
  :QWidget(parent,name)
{
  //
  // Initialize Variables
  //
  usi_mode=false;
  usi_config=config;
  usi_connect=connect;
  usi_output=output;

  //
  // Generate Fonts
  //
  QFont font("Helvetica",14,QFont::Normal);
  font.setPixelSize(14);
  QFont label_font("Helvetica",18,QFont::Bold);
  label_font.setPixelSize(18);
  QFont list_font("Helvetica",20,QFont::Normal);
  list_font.setPixelSize(20);
  QFont button_font("Helvetica",22,QFont::Bold);
  button_font.setPixelSize(22);
  QFont output_font("Helvetica",24,QFont::Bold);
  output_font.setPixelSize(24);

  //
  // The Output Label
  //
  usi_output_label=new QLabel(this,"usi_output_label");
  usi_output_label->setGeometry(0,0,sizeHint().width(),40);
  usi_output_label->setFont(output_font);
  usi_output_label->setAlignment(AlignCenter);

  //
  // The Input List
  //
  usi_input_box=new QComboBox(this,"usi_input_box");
  usi_input_box->setGeometry(0,40,sizeHint().width(),40);
  usi_input_box->setFont(list_font);
  QWidget::connect(usi_input_box,SIGNAL(activated(int)),
	  this,SLOT(inputActivatedData(int)));

  //
  // The Clear Button
  //
  usi_clear_button=new QPushButton("Clear",this,"usi_clear_button");
  usi_clear_button->setGeometry(0,90,sizeHint().width()/2-5,40);
  usi_clear_button->setFont(font);
  QWidget::connect(usi_clear_button,SIGNAL(clicked()),
		   this,SLOT(clearClickedData()));

  //
  // The Mode Button
  //
  usi_mode_button=new USIPushButton("Mode",this,"usi_mode_button");
  usi_mode_button->setGeometry(sizeHint().width()/2+5,90,
			       sizeHint().width()/2-5,40);
  usi_mode_button->setFont(font);
  usi_mode_button->setClockSource(USIPushButton::ExternalClock);
  this->connect(this,SIGNAL(clockTicked(bool)),
		usi_mode_button,SLOT(tickClock(bool)));
  usi_mode_button->setFlashColor(black);
  usi_mode_button->setFlashPeriod(300);
  QWidget::connect(usi_mode_button,SIGNAL(clicked()),
		   this,SLOT(modeClickedData()));

  //
  // The Take Button
  //
  usi_take_button=new USIPushButton("TAKE",this,"usi_take_button");
  usi_take_button->setGeometry(0,sizeHint().height()-50,
			       sizeHint().width(),50);
  usi_take_button->setFont(button_font);
  usi_take_button->setClockSource(USIPushButton::ExternalClock);
  this->connect(this,SIGNAL(clockTicked(bool)),
		usi_take_button,SLOT(tickClock(bool)));
  usi_take_button->setFlashColor(black);
  usi_take_button->setFlashPeriod(300);
  QWidget::connect(usi_take_button,SIGNAL(clicked()),
		   this,SLOT(takeClickedData()));

  //
  // The USI Connection
  //
  QWidget::connect(usi_connect,SIGNAL(inputListChanged()),
		   this,SLOT(inputListChangedData()));
  QWidget::connect(usi_connect,SIGNAL(outputListChanged()),
		   this,SLOT(outputListChangedData()));
  QWidget::connect(usi_connect,
		   SIGNAL(outputChanged(unsigned,std::vector<unsigned> *)),
		   this,
		   SLOT(outputChangedData(unsigned,std::vector<unsigned> *)));
}


USIOutput::~USIOutput()
{
}


QSize USIOutput::sizeHint() const
{
  return QSize(USI_OUTPUT_WIDTH,USI_OUTPUT_HEIGHT);
}


QSizePolicy USIOutput::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


QColor USIOutput::takeButtonColor() const
{
  return usi_take_button->flashColor();
}


void USIOutput::setTakeButtonColor(const QColor &color)
{
  QPalette pal=palette();
  pal.setColor(QPalette::Active,QColorGroup::ButtonText,color);
  pal.setColor(QPalette::Inactive,QColorGroup::ButtonText,color);
  usi_take_button->setPalette(pal);
  usi_take_button->setFlashColor(color);
}


void USIOutput::tickClock(bool state)
{
  emit clockTicked(state);
}


void USIOutput::inputListChangedData()
{
  usi_input_box->clear();
  usi_input_map.clear();
  if(usi_mode) {
    if(usi_config->allowInput(0)) {
      usi_input_box->insertItem(tr("- OFF -"));
      usi_input_map.push_back(0);
    }
    for(unsigned i=0;i<(usi_connect->inputs()-1);i++) {
      if(usi_config->allowInput(i+1)) {
	usi_input_box->insertItem(QString().sprintf("IN %04d",i+1));
	usi_input_map.push_back(i+1);
      }
    }
  }
  else {
    for(unsigned i=0;i<usi_connect->inputs();i++) {
      if(usi_config->allowInput(usi_connect->inputNumber(i))) {
	usi_input_box->insertItem(usi_connect->inputName(i,true));
	usi_input_map.push_back(i);
      }
    }
  }
}


void USIOutput::inputActivatedData(int index)
{
  if(usi_mode) {
    usi_pending_input=usi_input_map[index];
  }
  else {
    usi_pending_input=usi_connect->inputNumber(usi_input_map[index]);
  }
  usi_take_button->setFlashingEnabled(usi_pending_input!=usi_input);
}


void USIOutput::clearClickedData()
{
  if(usi_mode) {
    usi_input_box->setCurrentItem(GetInputIndex(usi_input));
  }
  else {
    usi_input_box->
      setCurrentItem(GetInputIndex(usi_connect->inputOffset(usi_input)));
  }
  usi_take_button->setFlashingEnabled(false);
  usi_pending_input=usi_input;
}


void USIOutput::modeClickedData()
{
  usi_mode=!usi_mode;
  usi_mode_button->setFlashingEnabled(usi_mode);
  inputListChangedData();
  outputListChangedData();
}


void USIOutput::takeClickedData()
{
  if(usi_pending_input!=usi_input) {
    usi_connect->switchCrosspoint(usi_pending_input,usi_output);
  }
}


void USIOutput::outputListChangedData()
{
  if(usi_output>=usi_connect->outputs()) {
    QMessageBox::warning(this,tr("Invalid Output"),
			 tr("The specified output doesn't exist!"));
    exit(256);
  }
  if(usi_mode) {
    usi_output_label->setText(QString().sprintf("OUT %04u",usi_output));
  }
  else {
    usi_output_label->
      setText(usi_connect->outputName(usi_output).stripWhiteSpace());
  }
  usi_connect->queryOutput(usi_output);
}


void USIOutput::outputChangedData(unsigned output,
				   std::vector<unsigned> *inputs)
{
  if(output!=usi_output) {
    return;
  }
  if(inputs->size()>1) {
    fprintf(stderr,"%u inputs mapped to output\n",inputs->size());
    return;
  }
  if(inputs->size()==0) {   // Output Off
    if(usi_mode) {
      usi_input_box->setCurrentItem(GetInputIndex(0));
    }
    else {
      usi_input_box->
	setCurrentItem(GetInputIndex(usi_connect->inputOffset(0)));
    }
    return;
  }
  usi_input=inputs->at(0);
  usi_pending_input=usi_input;
  if(usi_mode) {
    usi_input_box->setCurrentItem(GetInputIndex(usi_input));
  }
  else {
    usi_input_box->
      setCurrentItem(GetInputIndex(usi_connect->inputOffset(usi_input)));
  }
  usi_take_button->setFlashingEnabled(false);
}


int USIOutput::GetInputIndex(int val) const
{
  for(unsigned i=0;i<usi_input_map.size();i++) {
    if(usi_input_map[i]==val) {
      return i;
    }
  }
  return 0;
}
