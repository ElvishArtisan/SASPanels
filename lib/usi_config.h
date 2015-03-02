// usi_config.h
//
// A container class for SASPanels Configuration
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

#ifndef USI_CONFIG_H
#define USI_CONFIG_H

#include <qstring.h>
#include <qcolor.h>

//
// Default Configuration Values
//
#define USI_DEFAULT_CONF_FILE "/etc/saspanels.conf"
#define USI_DEFAULT_WIN_CONF_FILE "saspanels.ini"
#define USI_MAX_INPUTS 2048
#define USI_MAX_OUTPUTS 2048


class USIConfig
{
 public:
  USIConfig();
  USIConfig(const QString &filename);
  QString filename() const;
  void setFilename(const QString &filename);
  bool allowInput(unsigned input) const;
  QColor takeButtonColor() const;
  void load(unsigned output);
  void clear();

 private:
  QColor usi_take_button_color;
  bool usi_input_allowed[USI_MAX_INPUTS+1];
  QString usi_filename;
};


#endif  // USI_CONFIG_H
