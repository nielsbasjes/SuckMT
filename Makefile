#=========================================================================
#                 Copyright (C)1999-2006 by Niels Basjes
#              SuckMT Website : http://oss.basjes.nl/SuckMT
#                       Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Project: SuckMT, a multithreaded suck replacement
#  Language: make
#  $Id: Makefile 17 2008-08-17 15:29:35Z niels $
#  $Date: 2008-08-17 17:29:35 +0200 (Sun, 17 Aug 2008) $
#  $Revision: 17 $
#  $LastChangedBy: niels $
#=========================================================================
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   If you reuse code from SuckMT you are required to put a notice of 
#   this fact in both your manual and about box.
#
#=========================================================================

.PHONY: CopyrightMessage debug nodebug 

all: CopyrightMessage GNUmakefile
	${MAKE}

install: CopyrightMessage GNUmakefile
	${MAKE} install

dist: CopyrightMessage GNUmakefile
	${MAKE} dist

rpm: CopyrightMessage GNUmakefile
	${MAKE} rpm

full-distribution-set: CopyrightMessage GNUmakefile
	${MAKE} full-distribution-set

suckmt: CopyrightMessage GNUmakefile
	${MAKE} suckmt

suckmtstatic: CopyrightMessage GNUmakefile
	${MAKE} suckmtstatic 

debug: CopyrightMessage configure
	./configure --enable-debug
	rm -f Makefile
	cp Makefile.nognumake Makefile
	@echo The makefile has been created with debugging ENABLED
	@echo Run make again to actually start building suckmt.

nodebug: CopyrightMessage configure
	./configure --disable-debug
	rm -f Makefile
	cp Makefile.nognumake Makefile
	@echo The makefile has been created with debugging DISABLED
	@echo Run make again to actually start building suckmt.

aclocal.m4: configure.in
	aclocal

config.h.in:configure.in aclocal.m4
	autoheader

GNUmakefile.in: GNUmakefile.am configure.in config.h.in
	automake --add-missing GNUmakefile

configure: configure.in GNUmakefile.in aclocal.m4 
	autoconf
	
GNUmakefile: configure
	./configure
	rm -f Makefile
	cp Makefile.nognumake Makefile

configure.in: configure.in_ VERSION
	sed s/\#VERSION\#/`cat VERSION`/g < configure.in_ > configure.in

CopyrightMessage: VERSION
	@echo "+==================================================+"
	@echo "| ----------- GETTING READY TO BUILD ------------- |"
	@echo "+==================================================+"
	@echo "| SuckMT `cat VERSION` - A Multi Threaded suck replacement  |"
	@echo "| (C)2006 by Niels Basjes  http://suckmt.basjes.nl |"
	@echo "+--------------------------------------------------+"
	@echo "| SuckMT may be used under the GNU Public Licence. |"
	@echo "+==================================================+"
	@echo 
	@echo "Available targets to build:"
	@echo "- all     : Just build it all."
	@echo "- install : Build and install."
	@echo "- debug   : ENABLE  source debugging for all following compile sessions."
	@echo "- nodebug : DISABLE source debugging for all following compile sessions."
	@echo "- dist    : Create a suckmt-`cat VERSION`.tar.gz source distribution."
	@echo "- rpm     : Create an RPM and SRPM for suckmt."
	@echo 

clean:
	rm -f *.o core *.core suckmt suckmt-static
	rm -f configure configure.in config.{status,cache,log,h,h.in} aclocal.m4 stamp-h*
	rm -rf autom4te* depcomp install-sh missing
	rm -f GNUmakefile GNUmakefile.in 
	rm -f suckmt.lsm suckmt.spec suckmt.ini.sample suckmt.ini.sample.win32 
	rm -rf .deps
	cp Makefile.ed Makefile
