#=========================================================================
#                   Copyright (C) 2000 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.am
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : automake
#  $Date: 2000/03/14 19:45:38 $
#  $Revision: 1.21 $
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

bin_PROGRAMS = suckmt 
EXTRA_PROGRAMS = suckmtstatic

#-------------------------

# Optimization flags
#CXXFLAGS = -O0  -g
#CXXFLAGS = -O0 -g3 
CXXFLAGS = -O3 

#CFLAGS = -O0  -g
#CFLAGS = -O0 -g3 
CFLAGS = -O3 

#Uncomment to remove the symbols from the final executables
LD_OMIT_SYMBOLS = -s

#-------------------------


SUCKSOURCES  = main.cpp 
SUCKSOURCES += posix.cc 
SUCKSOURCES += SetProcTitle.cpp 
SUCKSOURCES += Abortable.cpp 
SUCKSOURCES += AsciiLineSocket.cpp 
SUCKSOURCES += Command.cpp 
SUCKSOURCES += CommandHandler.cpp 
SUCKSOURCES += CommandQueue.cpp 
SUCKSOURCES += GenericSocket.cpp 
SUCKSOURCES += IniFile.cpp 
SUCKSOURCES += NEWSArticle.cpp 
SUCKSOURCES += NNTPCommandHandler.cpp 
SUCKSOURCES += NNTPGetArticleCommand.cpp 
SUCKSOURCES += NNTPGetXOVERCommand.cpp 
SUCKSOURCES += NNTPProxy.cpp 
SUCKSOURCES += NNTPRetrieveManager.cpp 
SUCKSOURCES += ArticleImpactChecker.cpp
SUCKSOURCES += HeaderMatcher.cpp 
SUCKSOURCES += DuplicatesChecker.cpp 
SUCKSOURCES += NewsKiller.cpp 
SUCKSOURCES += regex.c
SUCKSOURCES += Printable.cpp 
SUCKSOURCES += StatisticsKeeper.cpp 
SUCKSOURCES += Tokenize.cpp
SUCKSOURCES += MultiStream.cpp 
SUCKSOURCES += TraceLog.cpp 

suckmt_SOURCES = $(SUCKSOURCES)
suckmtstatic_SOURCES = $(SUCKSOURCES)

suckmt_LDADD          = -lpthread 
suckmt_LDFLAGS        = $(LD_OMIT_SYMBOLS)
suckmtstatic_LDADD    = -lpthread
suckmtstatic_LDFLAGS  = $(LD_OMIT_SYMBOLS) -static

# Extradist includes some generic files
EXTRA_DIST  = Makefile.ed suckmt.ini.sample 
EXTRA_DIST += configure.in_ VERSION
EXTRA_DIST += suckmt.lsm.in  suckmt.lsm 
EXTRA_DIST += suckmt.spec.in suckmt.spec

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
EXTRA_DIST += SetProcTitle.h            SetProcTitle.cpp 
EXTRA_DIST += AsciiLineSocket.h         AsciiLineSocket.cpp 
EXTRA_DIST += Abortable.h               Abortable.cpp 
EXTRA_DIST += Command.h                 Command.cpp 
EXTRA_DIST += CommandHandler.h          CommandHandler.cpp 
EXTRA_DIST += CommandQueue.h            CommandQueue.cpp 
EXTRA_DIST += GenericSocket.h           GenericSocket.cpp 
EXTRA_DIST += IniFile.h                 IniFile.cpp 
EXTRA_DIST += NEWSArticle.h             NEWSArticle.cpp 
EXTRA_DIST += NNTPCommandHandler.h      NNTPCommandHandler.cpp 
EXTRA_DIST += NNTPGetArticleCommand.h   NNTPGetArticleCommand.cpp 
EXTRA_DIST += NNTPGetXOVERCommand.h     NNTPGetXOVERCommand.cpp 
EXTRA_DIST += NNTPProxy.h               NNTPProxy.cpp 
EXTRA_DIST += NNTPRetrieveManager.h     NNTPRetrieveManager.cpp 
EXTRA_DIST += ArticleImpactChecker.h    ArticleImpactChecker.cpp 
EXTRA_DIST += HeaderMatcher.h           HeaderMatcher.cpp 
EXTRA_DIST += regex.h                   regex.c
EXTRA_DIST += DuplicatesChecker.h       DuplicatesChecker.cpp 
EXTRA_DIST += NewsKiller.h              NewsKiller.cpp 
EXTRA_DIST += Printable.h               Printable.cpp 
EXTRA_DIST += StatisticsKeeper.h        StatisticsKeeper.cpp 
EXTRA_DIST += Tokenize.h                Tokenize.cpp 
EXTRA_DIST += MultiStream.h             MultiStream.cpp 
EXTRA_DIST += TraceLog.h                TraceLog.cpp 

# Enable Compiler warnings
ALLWARNINGS = -Wall -W  -Wtraditional -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual -Wsynth 
CXXFLAGS += $(ALLWARNINGS)
#CFLAGS += $(ALLWARNINGS)

#Some flags for the regular expression library
CXXFLAGS += -DSTDC_HEADERS -DREGEX_MALLOC

#echo @$(PLATFORM)@

UNAME=`uname`

#ifeq ($(UNAME),Linux)
CXXFLAGS += -D__x86__ -D__linux__ -D__OSVERSION__=2 -DNoNanoSleep -D_REENTRANT -DPthreadDraftVersion=10
#endif

# IRIX FLAGS TO BE CHECKED
#ifeq ($(UNAME),IRIX)
#CXXFLAGS+=-D__irix__ -DNoNanoSleep -D_REENTRANT -DPthreadDraftVersion=10
#endif

cxxflags:
	echo @$(CXXFLAGS)@

# This target makes the distribution easier to build for the end user.
dist-hook: suckmt.lsm suckmt.spec
	rm $(distdir)/configure
	rm $(distdir)/Makefile.in
	cp Makefile.ed $(distdir)/Makefile

configure.in: configure.in_ VERSION
	sed s/VERSION/`cat VERSION`/g < configure.in_ > configure.in

suckmt.spec: suckmt.spec.in configure.in
	sed "s/\#VERSION\#/$(VERSION)/g" < suckmt.spec.in > suckmt.spec

suckmt.lsm: suckmt.lsm.in configure.in
	cat suckmt.lsm.in \
        | sed "s/\#VERSION\#/$(VERSION)/g" \
        | sed "s/\#DATE\#/`date +%Y-%m-%d`/g" > suckmt.lsm

rpm: suckmt.lsm suckmt.spec dist
	rpm -ta $(distdir).tar.gz 
	mv /usr/src/*/RPMS/i386/suckmt*.rpm ~
	mv /usr/src/*/SRPMS/suckmt*.rpm ~

#End of the file Makefile.am
#=========================================================================
