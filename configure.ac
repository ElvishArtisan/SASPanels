dnl configure.ac
dnl
dnl Autoconf configuration for SASPanels.
dnl
dnl Use autoconf to process this into a configure script
dnl
dnl   (C) Copyright 2002-2006,2015 Fred Gleason <fredg@paravelsystems.com>
dnl
dnl   This program is free software; you can redistribute it and/or modify
dnl   it under the terms of the GNU General Public License as
dnl   published by the Free Software Foundation; either version 2 of
dnl   the License, or (at your option) any later version.
dnl
dnl   This program is distributed in the hope that it will be useful,
dnl   but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl   GNU General Public License for more details.
dnl
dnl   You should have received a copy of the GNU General Public
dnl   License along with this program; if not, write to the Free Software
dnl   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
dnl

AC_INIT([saspanels],m4_esyscmd(cat PACKAGE_VERSION))
AM_INIT_AUTOMAKE
AC_SUBST(RPM_RELEASE,1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])
AC_CONFIG_MACRO_DIR([m4])

#
# Some Fully Resolved Install Paths
# (for the packaging system and friends)
#
if test ${prefix} = "NONE" ; then
  AC_SUBST(LOCAL_PREFIX,/usr/local)
else
  AC_SUBST(LOCAL_PREFIX,${prefix})
fi

#
# Basic Compiler Checks
#
AC_PROG_CXX
AC_PROG_LIBTOOL

# to avoid rpath usage :
# http://wiki.debian.net/index.cgi?RpathIssue
case ${host} in
  *-pc-linux-gnu)
    AC_MSG_RESULT([Fixing libtool for -rpath problems.])
    sed < libtool > libtool-2 \
    's/^hardcode_libdir_flag_spec.*$/hardcode_libdir_flag_spec=" -D__LIBTOOL_IS_A_FOOL__ "/'
    mv libtool-2 libtool
    chmod 755 libtool
  ;;
esac

#
# Check for Qt
#
BNV_HAVE_QT
if test $have_qt = "no" ; then
  AC_MSG_ERROR([*** Qt toolkit not found, unable to continue ***])
fi

#
# Determine the target architecture
#
AR_GCC_TARGET()
AC_SUBST(VENDOR,$ar_gcc_distro)
AC_SUBST(ARCH,$ar_gcc_arch)

#
# Determine Distro
#
AR_GET_DISTRO()
if test $ar_gcc_distro = suse ; then
  if test `echo $ar_distro_version | sed "s/\.//"` -lt 103 ; then
    AC_SUBST(CDDA_DEPS,cdda2wav)
  else
    AC_SUBST(CDDA_DEPS,cdrkit-cdrtools-compat)
  fi
else
  AC_SUBST(CDDA_DEPS,cdda2wav)
fi

#
# Check for OggVorbis
#
AC_CHECK_HEADER(vorbis/vorbisfile.h,[VORBIS_FOUND=yes],[])
if test $VORBIS_FOUND = yes ; then
  AC_DEFINE(HAVE_VORBIS)
  AC_SUBST(LIBVORBIS,"-lvorbisfile -lvorbisenc")
else
  AC_SUBST(LIBVORBIS,"")
fi

#
# Configure RPM Build
#
AC_CHECK_PROG(RPMBUILD_FOUND,rpmbuild,[yes],[])
if test -z $RPMBUILD_FOUND ; then
  AC_SUBST(RPMBUILD,rpm)
else
  AC_SUBST(RPMBUILD,rpmbuild)
fi
if test -d /usr/src/redhat ; then
  AC_SUBST(RPM_ROOT,/usr/src/redhat)
  AC_SUBST(VENDOR,redhat)
else
  AC_SUBST(RPM_ROOT,/usr/src/packages)
  AC_SUBST(VENDOR,suse)
fi 


AC_OUTPUT(Makefile \
          lib/Makefile \
          sas_single/Makefile \
	  icons/Makefile \
	  saspanels.spec \
	  xdg/Makefile \
)

#
# Configuration Results
#
AC_MSG_NOTICE()
AC_MSG_NOTICE(Now enter 'make' to build the software.)
AC_MSG_NOTICE()
