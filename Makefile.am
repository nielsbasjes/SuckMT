#=========================================================================
#                   Copyright (C) 1999 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.am
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : automake
#  $Date: 1999/11/19 21:52:06 $
#  $Revision: 1.5 $
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

EXTRA_DIST = AsciiLineSocket.cpp AsciiLineSocket.h Command.cpp Command.h CommandHandler.cpp CommandHandler.h CommandQueue.cpp CommandQueue.h GenericSocket.cpp GenericSocket.h NEWSArticle.cpp NEWSArticle.h NNTPCommandHandler.cpp NNTPCommandHandler.h NNTPGetArticleCommand.cpp NNTPGetArticleCommand.h NNTPGetXOVERCommand.cpp NNTPGetXOVERCommand.h NNTPProxy.cpp NNTPProxy.h NNTPRetrieveManager.cpp NNTPRetrieveManager.h NewsKiller.cpp NewsKiller.h Printable.cpp Printable.h TraceLog.cpp TraceLog.h main.cpp omnithread.h posix.cc tsqueue.h StatisticsKeeper.cpp StatisticsKeeper.h Abortable.cpp Abortable.h IniFile.cpp IniFile.h Tokenize.cpp Tokenize.h

# Optimization flags
#CXXFLAGS=-O0  -g
#CXXFLAGS=-O0 -g3 
CXXFLAGS=-O3 

# Enable Compiler warnings
#CXXFLAGS+= -Wall 
CXXFLAGS+= -Wall -W  -Wtraditional -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual -Wsynth 

# Linux specific omnithread flags
CXXFLAGS+=-D__x86__ -D__linux__ -D__OSVERSION__=2 -DNoNanoSleep -D_REENTRANT -DPthreadDraftVersion=10

easydist: distclean
	rm configure
	rm Makefile.in
	cp Makefile.ed Makefile

# End of the file Makefile.am
#=========================================================================
