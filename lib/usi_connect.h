// usi_connect.h
//
// A connection driver for USI devices.
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

#ifndef USI_CONNECT_H
#define USI_CONNECT_H

#include <vector>
#include <map>

#include <qobject.h>
#include <qsocket.h>

#include <usi.h>

class USIConnect : public QObject
{
 Q_OBJECT
 public:
  enum SwitchAction {Take=0,Sum=1,DirectRelay=2};
  enum ConnectStatus {Connected=0,DnsFailed=1,Refused=2,Other=3};
  USIConnect(QObject *parent=0,const char *name=0);
  ~USIConnect();
  void connectToHost(QString hostname,Q_UINT16 hostport);
  unsigned inputs() const;
  unsigned inputNumber(unsigned offset);
  unsigned inputOffset(unsigned input);
  QString inputName(unsigned pos,bool alphabetical=false);
  unsigned outputs() const;
  unsigned outputNumber(unsigned pos);
  unsigned outputOffset(unsigned input);
  QString outputName(unsigned pos,bool alphabetical=false);
  void queryOutput(unsigned output);
  void switchCrosspoint(unsigned input,unsigned output,int gain=0,
			SwitchAction action=USIConnect::Take);

 signals:
  void connectionStatusChanged(USIConnect::ConnectStatus status);
  void inputListChanged();
  void outputListChanged();
  void outputChanged(unsigned output,std::vector<unsigned> *inputs);

 private slots:
  void connectionClosedData();
  void readyReadData();
  void errorData(int err);

 private:
  void DispatchCommand();
  void BubbleSort(std::map<unsigned,QString> *names,
		  std::vector<unsigned> *ptrs);
  QSocket *usi_socket;
  char usi_buffer[USI_MAX_LENGTH];
  unsigned usi_ptr;
  std::map<unsigned,QString> usi_input_names;
  std::vector<unsigned> usi_input_ptrs;
  std::map<unsigned,QString> usi_output_names;
  std::vector<unsigned> usi_output_ptrs;
  bool usi_started;
};


#endif  // USI_CONNECT_H
