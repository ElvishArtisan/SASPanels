## automake.am
##
## Top level automake.am for SASPanels
##
## Use automake to process this into a Makefile.in
##
##   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
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

SUBDIRS = lib\
          icons\
          sas_single\
          xdg

ACLOCAL_AMFLAGS = -I m4

maintainer-clean-local:	
	rm -rf m4

rpm:	dist
	mkdir -p $(HOME)/rpmbuild/SOURCES
	cp saspanels-$(VERSION).tar.gz $(HOME)/rpmbuild/SOURCES/
	$(RPMBUILD) -ba --target $(ARCH)-$(VENDOR)-linux saspanels.spec
	mv $(HOME)/rpmbuild/RPMS/$(ARCH)/saspanels-*.rpm $(top_srcdir)/
	mv $(HOME)/rpmbuild/SRPMS/saspanels-*.src.rpm $(top_srcdir)/
	rm $(HOME)/rpmbuild/SOURCES/saspanels-$(VERSION).tar.gz

zip:	dist
	tar -zxf saspanels-@VERSION@.tar.gz
	zip -r saspanels-@VERSION@.zip saspanels-@VERSION@
	rm -rf saspanels-@VERSION@

EXTRA_DIST = autogen.sh saspanels.conf-sample saspanels.spec.in\
             saspanels.pro saspanels.ini gpl.rtf PACKAGE_VERSION \
             build_win32.bat get_distro.sh get_target.sh saspanels.ism

CLEANFILES = *~ *.tar.gz *.rpm *.zip
DISTCLEANFILES = saspanels.spec
MAINTAINERCLEANFILES = *~ Makefile.in configure aclocal.m4 *.tar.gz\
                       config.guess config.status config.sub install-sh\
                       libtool ltmain.sh missing mkinstalldirs depcomp\
                       saspanels.spec
