#=========================================================================
#                   Copyright (C) 1999 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.ed
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : make
#  $Date: 2002/12/27 13:24:00 $
#  $Revision: 1.24 $
#  $RCSfile: Makefile.ed,v $
#  $Author: niels $
#=========================================================================
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
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
	@echo "| (C)2002 by Niels Basjes  -  http://go.to/suckmt  |"
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
