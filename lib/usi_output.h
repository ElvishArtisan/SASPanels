// usi_output.h
//
// A USI-based single-output input selector.
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


#ifndef USL_OUTPUT_H
#define USL_OUTPUT_H

#include <vector>

#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>

#include <usi_pushbutton.h>
#include <usi_config.h>
#include <usi_connect.h>

#define USI_OUTPUT_WIDTH 140
#define USI_OUTPUT_HEIGHT 190
#define USI_OUTPUT_FLASH_INTERVAL 300

class USIOutput : public QWidget
{
  Q_OBJECT
 public:
  USIOutput(USIConfig *config,USIConnect *connect,unsigned output,
	    QWidget *parent=0,const char *name=0);
  ~USIOutput();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  QColor takeButtonColor() const;
  void setTakeButtonColor(const QColor &color);

 public slots:
  void tickClock(bool state);

 signals:
  void clockTicked(bool state);

 private slots:
  void inputListChangedData();
  void outputListChangedData();
  void outputChangedData(unsigned output,std::vector<unsigned> *inputs);
  void inputActivatedData(int index);
  void clearClickedData();
  void modeClickedData();
  void takeClickedData();

 private:
  int GetInputIndex(int val) const;
  USIConfig *usi_config;
  USIConnect *usi_connect;
  QLabel *usi_output_label;
  QComboBox *usi_input_box;
  QPushButton *usi_clear_button;
  USIPushButton *usi_mode_button;
  USIPushButton *usi_take_button;
  unsigned usi_output;
  unsigned usi_input;
  unsigned usi_pending_input;
  bool usi_mode;
  std::vector<int> usi_input_map;
};


#endif  // USI_OUTPUT_H
