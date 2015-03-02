// sas_single.cpp
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

#include <stdlib.h>

#include <qapplication.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qsettings.h>

#include <usi_connect.h>
#include <usi_cmdswitch.h>
#include <sas_single.h>

//
// Icons
//
#include "../icons/saspanels-22x22.xpm"


MainWidget::MainWidget(QWidget *parent,const char *name)
  :QWidget(parent,name)
{
  QString config_filename;

  //
  // Initialize Variables
  //
  int output=0;
  QString hostname;
  bool ok;
  unsigned cols=0;
  int ypos=0;
  USIConfig *config=NULL;
  panel_quantity=0;
  usi_columns=0;
  usi_rows=0;
  setCaption("SASPanels");

  //
  // Read Command Options
  //
  USICmdSwitch *cmd=
    new USICmdSwitch(qApp->argc(),qApp->argv(),"usi_single",USI_SINGLE_USAGE);

  //
  // Create And Set Icon
  //
  usi_saspanels_map=new QPixmap(saspanels_22x22_xpm);
  setIcon(*usi_saspanels_map);

  //
  // Determine Configuration File Location
  //
#ifdef WIN32
  QSettings settings=QSettings();
  settings.insertSearchPath(QSettings::Windows,"/SalemRadioLabs");
  config_filename=
    QString().sprintf("%s\\saspanels.ini",
		      (const char *)settings.
		      readEntry("/SASPanels/InstallDir","C:\\etc"));
#else
  config_filename=QString(USI_DEFAULT_CONF_FILE);
#endif  // WIN32

  //
  // Get Host Address and Columns Values
  //
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--ip-address") {
      hostname=cmd->value(i);
    }
    if(cmd->key(i)=="--columns") {
      usi_columns=cmd->value(i).toUInt();
    }
  }
  if(hostname.isEmpty()) {
    QMessageBox::warning(this,"SASPanels",tr("Missing hostname!"));
    exit(256);
  }

  //
  // The USI Connection
  //
  USIConnect *connect=new USIConnect(this,"connection");
  QWidget::connect(connect,
	  SIGNAL(connectionStatusChanged(USIConnect::ConnectStatus)),
	  this,
	  SLOT(connectionStatusChangedData(USIConnect::ConnectStatus)));

  //
  // The Button Clock
  //
  clock_state=false;
  QTimer *timer=new QTimer(this,"button_flash_timer");
  timer->start(USI_OUTPUT_FLASH_INTERVAL);
  this->connect(timer,SIGNAL(timeout()),this,SLOT(tickClock()));

  //
  // Start Output Panels
  //
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--output") {
      output=cmd->value(i).toInt(&ok);
      if((ok)&&(output>0)) {
	config=new USIConfig(config_filename);
	config->load(output);
	USIOutput *widget=new USIOutput(config,connect,output,this,"widget");
	this->connect(this,SIGNAL(clockTicked(bool)),
		      widget,SLOT(tickClock(bool)));
	widget->setTakeButtonColor(config->takeButtonColor());
	if(usi_columns==0) {
	  widget->
	    setGeometry(10+panel_quantity*(20+widget->sizeHint().width()),0,
			widget->sizeHint().width(),
			widget->sizeHint().height());
	}
	else {
	  widget->
	    setGeometry(10+cols*(20+widget->sizeHint().width()),ypos,
			widget->sizeHint().width(),
			widget->sizeHint().height());
	  cols++;
	  if(cols==usi_columns) {
	    cols=0;
	    usi_rows++;
	    ypos+=(USI_OUTPUT_HEIGHT+10);
	  }
	}
	panel_quantity++;
      }
    }
  }
  if(cols!=0) {
    usi_rows++;
  }
  if(panel_quantity==0) {
    QMessageBox::warning(this,"SASPanels",tr("No valid outputs to display!"));
    exit(256);
  }

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  //
  // Fire up the USI connection
  //
  connect->connectToHost(hostname,1200);

  delete cmd;
}


MainWidget::~MainWidget()
{
}


QSize MainWidget::sizeHint() const
{
  if(usi_columns==0) {
    return QSize((USI_OUTPUT_WIDTH+20)*panel_quantity,USI_OUTPUT_HEIGHT+10);
  }
  else {
    return QSize((USI_OUTPUT_WIDTH+20)*usi_columns,
		 (USI_OUTPUT_HEIGHT+10)*usi_rows);
  }
}


void MainWidget::connectionStatusChangedData(USIConnect::ConnectStatus status)
{
  switch(status) {
      case USIConnect::Refused:
	QMessageBox::warning(this,tr("Connection Refused"),
			     tr("The connection was refused by the server!"));
	exit(256);

      case USIConnect::DnsFailed:
	QMessageBox::warning(this,tr("No Server"),
			     tr("The specified server name cannot be found!"));
	exit(256);

      case USIConnect::Other:
	QMessageBox::warning(this,tr("Network Error"),
			     tr("A network error occurred!"));
	exit(256);

      default:
	break;
  }
}


void MainWidget::tickClock()
{
  emit clockTicked(clock_state);
  clock_state=!clock_state;
}


QSizePolicy MainWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MainWidget::paintEvent(QPaintEvent *e)
{
  QPainter *p=new QPainter(this);
  p->setPen(black);
  p->setBrush(black);
  if(usi_columns==0) {
    for(unsigned i=1;i<panel_quantity;i++) {
      p->moveTo(i*(USI_OUTPUT_WIDTH+20),0);
      p->lineTo(i*(USI_OUTPUT_WIDTH+20),sizeHint().height());
    }
  }
  else {
    for(unsigned i=1;i<usi_columns;i++) {
      p->moveTo(i*(USI_OUTPUT_WIDTH+20),0);
      p->lineTo(i*(USI_OUTPUT_WIDTH+20),sizeHint().height());
    }
    for(unsigned i=1;i<usi_rows;i++) {
      p->moveTo(0,i*(USI_OUTPUT_HEIGHT+10)-1);
      p->lineTo(sizeHint().width(),i*(USI_OUTPUT_HEIGHT+10)-1);
    }
  }
  delete p;
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);

  //
  // Start Event Loop
  //
  MainWidget *w=new MainWidget(NULL,"main");
  a.setMainWidget(w);
  w->setGeometry(w->geometry().x(),w->geometry().y(),w->sizeHint().width(),w->sizeHint().height());
  w->show();
  return a.exec();
}
