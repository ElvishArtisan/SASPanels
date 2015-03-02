# lib.pro
#
# The lib/ QMake project file for SASPanels.
#
# (C) Copyright 2003-2006 Fred Gleason <fredg@paravelsystems.com>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

TEMPLATE = lib

win32 {
  DEFINES += WIN32
  DEFINES += VERSION=\"$$[VERSION]\"
  DEFINES += PACKAGE=\"saspanels\" 
  DEFINES += PACKAGE_VERSION=\"$$[VERSION]\" 
  DEFINES += PACKAGE_NAME=\"saspanels\"
  DEFINES += PACKAGE_BUGREPORT=\"fredg@paravelsystems.com\"
}

SOURCES += usi_output.cpp
SOURCES += usi_connect.cpp
SOURCES += usi_config.cpp
SOURCES += usi_cmdswitch.cpp
SOURCES += usi_profile.cpp
SOURCES += usi_profilesection.cpp
SOURCES += usi_profileline.cpp
SOURCES += usi_pushbutton.cpp

HEADERS += usi_output.h
HEADERS += usi_connect.h
HEADERS += usi_config.h
HEADERS += usi_cmdswitch.h
HEADERS += usi.h
HEADERS += usi_profile.h
HEADERS += usi_profilesection.h
HEADERS += usi_profileline.h
HEADERS += usi_pushbutton.h

CONFIG += qt staticlib
