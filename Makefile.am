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

SUBDIRS = lib sas_single icons xdg

rpm:	dist
	cp saspanels-$(VERSION).tar.gz $(RPM_ROOT)/SOURCES/
	$(RPMBUILD) -ba --target i586-$(VENDOR)-linux saspanels.spec
	mv $(RPM_ROOT)/RPMS/i586/saspanels-$(VERSION)-@RPM_RELEASE@.i586.rpm $(top_srcdir)/
	mv $(RPM_ROOT)/SRPMS/saspanels-$(VERSION)-@RPM_RELEASE@.src.rpm $(top_srcdir)/
	rm $(RPM_ROOT)/SOURCES/saspanels-$(VERSION).tar.gz
	rm -rf $(RPM_ROOT)/BUILD/saspanels-$(VERSION)

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
