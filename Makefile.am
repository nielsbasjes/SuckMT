#=========================================================================
#                   Copyright (C) 1999 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.am
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : automake
#  $Date: 1999/12/13 21:02:04 $
#  $Revision: 1.7 $
#  $RCSfile: Makefile.am,v $
#  $Author: niels $
#=========================================================================
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#=========================================================================

bin_PROGRAMS = suckmt suckmtstatic

suckmt_SOURCES = IniFile.cpp Abortable.cpp StatisticsKeeper.cpp posix.cc main.cpp TraceLog.cpp Printable.cpp NewsKiller.cpp NNTPRetrieveManager.cpp NNTPProxy.cpp NNTPGetXOVERCommand.cpp NNTPGetArticleCommand.cpp NNTPCommandHandler.cpp NEWSArticle.cpp GenericSocket.cpp CommandQueue.cpp CommandHandler.cpp Command.cpp AsciiLineSocket.cpp Tokenize.cpp
suckmtstatic_SOURCES = IniFile.cpp Abortable.cpp StatisticsKeeper.cpp posix.cc main.cpp TraceLog.cpp Printable.cpp NewsKiller.cpp NNTPRetrieveManager.cpp NNTPProxy.cpp NNTPGetXOVERCommand.cpp NNTPGetArticleCommand.cpp NNTPCommandHandler.cpp NEWSArticle.cpp GenericSocket.cpp CommandQueue.cpp CommandHandler.cpp Command.cpp AsciiLineSocket.cpp Tokenize.cpp

suckmt_LDADD          = -lpthread
suckmtstatic_LDADD    = -lpthread 
suckmtstatic_LDFLAGS  =  -static

# Extradist includes some generic files
EXTRA_DIST  = Makefile.ed suckmt.ini.sample suckmt.lsm 

# The MS-Visual Studio Specific Project/Make files.
EXTRA_DIST += SuckMT.dsp SuckMT.dsw SuckMT.plg 

# The omni_thread sources for all platforms
EXTRA_DIST += omnithread.h 
EXTRA_DIST += omnithread/mach.h     mach.cc 
EXTRA_DIST += omnithread/nt.h       nt.cpp
EXTRA_DIST += omnithread/posix.h    posix.cc 
EXTRA_DIST += omnithread/solaris.h  solaris.cc 

# All the C++ sources and header files.
EXTRA_DIST += SuckDefines.h main.cpp tsqueue.h  
EXTRA_DIST += AsciiLineSocket.h         AsciiLineSocket.cpp 
EXTRA_DIST += Abortable.h               Abortable.cpp 
EXTRA_DIST += Command.h                 Command.cpp 
EXTRA_DIST += CommandHandler.h  	    CommandHandler.cpp 
EXTRA_DIST += CommandQueue.h            CommandQueue.cpp 
EXTRA_DIST += GenericSocket.h           GenericSocket.cpp 
EXTRA_DIST += IniFile.h                 IniFile.cpp 
EXTRA_DIST += NEWSArticle.h             NEWSArticle.cpp 
EXTRA_DIST += NNTPCommandHandler.h      NNTPCommandHandler.cpp 
EXTRA_DIST += NNTPGetArticleCommand.h   NNTPGetArticleCommand.cpp 
EXTRA_DIST += NNTPGetXOVERCommand.h     NNTPGetXOVERCommand.cpp 
EXTRA_DIST += NNTPProxy.h               NNTPProxy.cpp 
EXTRA_DIST += NNTPRetrieveManager.h     NNTPRetrieveManager.cpp 
EXTRA_DIST += NewsKiller.h              NewsKiller.cpp 
EXTRA_DIST += Printable.h               Printable.cpp 
EXTRA_DIST += StatisticsKeeper.h        StatisticsKeeper.cpp 
EXTRA_DIST += Tokenize.h                Tokenize.cpp 
EXTRA_DIST += TraceLog.h                TraceLog.cpp 

# Optimization flags
#CXXFLAGS=-O0  -g
#CXXFLAGS=-O0 -g3 
CXXFLAGS=-O3 

# Enable Compiler warnings
CXXFLAGS+= -Wall -W  -Wtraditional -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual -Wsynth 

# Linux specific omnithread flags
CXXFLAGS+=-D__x86__ -D__linux__ -D__OSVERSION__=2 -DNoNanoSleep -D_REENTRANT -DPthreadDraftVersion=10

# This target makes the distribution easier to build for the end user.
easydist: distdir
	-chmod -R a+r $(distdir)
	rm $(distdir)/configure
	rm $(distdir)/Makefile.in
	cp Makefile.ed $(distdir)/Makefile
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)

# End of the file Makefile.am
#=========================================================================

