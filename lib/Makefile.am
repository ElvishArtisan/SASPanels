## automake.am
##
## lib/ Automake.am for SASPanels
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2002-2005 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as
##   published by the Free Software Foundation; either version 2 of
##   the License, or (at your option) any later version.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##

AM_CPPFLAGS = -Wall
INCLUDES = -I$(QTDIR)/include
MOC = $(QTDIR)/bin/moc

# QT's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@


lib_LTLIBRARIES = libsaspanels.la

dist_libsaspanels_la_SOURCES = usi_output.cpp usi_output.h\
                               usi_connect.cpp usi_connect.h\
                               usi_config.cpp usi_config.h\
                               usi_cmdswitch.cpp usi_cmdswitch.h\
                               usi_profile.cpp usi_profile.h\
                               usi_profileline.cpp usi_profileline.h\
                               usi_profilesection.cpp usi_profilesection.h\
                               usi_pushbutton.cpp usi_pushbutton.h\
                               usi.h

nodist_libsaspanels_la_SOURCES = moc_usi_output.cpp moc_usi_connect.cpp\
                                 moc_usi_pushbutton.cpp

libsaspanels_la_LDFLAGS = -release $(VERSION)

EXTRA_DIST = lib.pro

CLEANFILES = *~ moc_* *.tar.gz *.obj *.lib *.idb *.pdb *.ilk
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 moc_* *.tar.gz
