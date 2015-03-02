// usi_config.cpp
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

#include <usi_profile.h>

#include <usi_config.h>


USIConfig::USIConfig()
{
  clear();
}


USIConfig::USIConfig(const QString &filename)
{
  clear();
  usi_filename=filename;
}


QString USIConfig::filename() const
{
  return usi_filename;
}


void USIConfig::setFilename(const QString &filename)
{
  usi_filename=filename;
}


bool USIConfig::allowInput(unsigned input) const
{
  return usi_input_allowed[input];
}


QColor USIConfig::takeButtonColor() const
{
  return usi_take_button_color;
}


void USIConfig::load(unsigned output)
{
  QString section;
  bool default_action=true;

  USIProfile *profile=new USIProfile();
  profile->setSource(usi_filename);

  section=QString().sprintf("Output%d",output);
  usi_take_button_color=QColor(profile->stringValue(section,"TakeButtonColor",
					       usi_take_button_color.name()));
  default_action=
    profile->stringValue(section,"DefaultAction").lower()!="deny";
  for(unsigned i=0;i<(USI_MAX_INPUTS+1);i++) {
    usi_input_allowed[i]=
      profile->boolValue(section,
			 QString().sprintf("AllowInput%d",i),default_action);
  }
  delete profile;
}


void USIConfig::clear()
{
  usi_filename="";
  usi_take_button_color=QColor("black");
  for(unsigned i=0;i<USI_MAX_INPUTS;i++) {
    usi_input_allowed[i]=true;
  }
}
