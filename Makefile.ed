#=========================================================================
#                   Copyright (C) 1999 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.ed
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : make
#  $Date: 1999/12/13 22:16:30 $
#  $Revision: 1.4 $
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

easydist: TheMakefile
	${MAKE} easydist
    
TheMakefile:
	automake
	autoconf
	./configure
