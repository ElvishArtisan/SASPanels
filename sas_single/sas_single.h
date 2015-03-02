// usi_single.h
//
// An applet for controling a USI output.
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


#ifndef USI_SINGLE_H
#define USI_SINGLE_H

#include <qwidget.h>
#include <qpixmap.h>

#include <usi_output.h>

#define USI_SINGLE_USAGE "[columns=<cols>] --ip-address=<ip-addr> --output=<output-num> [...]\n\nUse the SAS USI server at <ip-addr>.  Multiple '--output' arguments may\nbe specified.  Optionally, '--columns' specifies the maximum number of\noutput controls to tile horizontally before starting a new row.\n"

class MainWidget : public QWidget
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0,const char *name=0);
  ~MainWidget();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 signals:
  void clockTicked(bool state);

 private slots:
  void connectionStatusChangedData(USIConnect::ConnectStatus status);
  void tickClock();

 protected:
  void paintEvent(QPaintEvent *e);

 private:
  unsigned panel_quantity;
  unsigned usi_columns;
  unsigned usi_rows;
  bool clock_state;
  QPixmap *usi_saspanels_map;
};


#endif  // USI_SINGLE_H
