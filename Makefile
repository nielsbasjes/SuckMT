#=========================================================================
#                   Copyright (C) 1999 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.ed
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : make
#  $Date: 2000/03/13 21:39:08 $
#  $Revision: 1.10 $
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

.PHONY: TheMakefile

all: TheMakefile
	${MAKE}

dist: TheMakefile
	${MAKE} dist

rpm: TheMakefile
	${MAKE} rpm
    
configure: configure.in
	aclocal
	automake
	autoconf
	
TheMakefile: configure
	./configure

configure.in: configure.in_ VERSION
	sed s/VERSION/`cat VERSION`/g < configure.in_ > configure.in
