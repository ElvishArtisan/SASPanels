## automake.am
##
## sas_single(1) Makefile.am for SASPanels
##
## by Fred Gleason <fredg@paravelsystems.com>
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

AM_CPPFLAGS = -Wall -DQTDIR=\"@QT_DIR@\" @QT_CXXFLAGS@ -I$(QTDIR)/include -I../lib
LIBS = @QT_LIBS@ -L$(QTDIR)/lib -L../lib
MOC = @QT_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@


bin_PROGRAMS = sas_single

dist_sas_single_SOURCES = sas_single.cpp sas_single.h
nodist_sas_single_SOURCES = moc_sas_single.cpp
sas_single_LDADD = -lsaspanels

EXTRA_DIST = sas_single.pro

CLEANFILES = *~ moc_* *.obj *.exe
MAINTAINERCLEANFILES = *~ Makefile.in
