#=========================================================================
#                   Copyright (C) 1999 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.ed
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : make
#  $Date: 2000/03/20 21:32:35 $
#  $Revision: 1.13 $
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

.PHONY: CopyrightMessage debug nodebug TheMakefile

all: CopyrightMessage TheMakefile
	${MAKE}

install: CopyrightMessage TheMakefile
	${MAKE} dist

debug: CopyrightMessage configure
	./configure --enable-debug
	@echo The makefile has been created with debugging ENABLED
	@echo Run make again to actually start building suckmt.

nodebug: CopyrightMessage configure
	./configure --disable-debug
	@echo The makefile has been created with debugging DISABLED
	@echo Run make again to actually start building suckmt.

dist: CopyrightMessage TheMakefile
	${MAKE} dist

rpm: CopyrightMessage TheMakefile
	${MAKE} rpm

aclocal.m4: configure.in
	aclocal

Makefile.in: Makefile.am configure.in config.h.in
	automake

configure: configure.in Makefile.in aclocal.m4
	autoconf
	
TheMakefile: configure
	./configure

configure.in: configure.in_ VERSION
	sed s/VERSION/`cat VERSION`/g < configure.in_ > configure.in

CopyrightMessage: VERSION
	@echo "+==================================================+"
	@echo "| ----------- GETTING READY TO BUILD ------------- |"
	@echo "+==================================================+"
	@echo "| Suck MT `cat VERSION` - A Multi Threaded suck replacement |"
	@echo "| (C)2000 by Niels Basjes  -  http://go.to/suckmt  |"
	@echo "+--------------------------------------------------+"
	@echo "| SuckMT may be used under the GNU Public Licence. |"
	@echo "+==================================================+"
	@echo 
	@echo "Available targets to build:"
	@echo "- all     : Just build it all."
	@echo "- debug   : ENABLE  source debugging for all following compile sessions."
	@echo "- nodebug : DISABLE source debugging for all following compile sessions."
	@echo "- dist    : Create a suckmt.`cat VERSION`.tar.gz source distribution."
	@echo "- rpm     : Create an RPM and SRPM for suckmt."
	@echo 
