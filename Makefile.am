#=========================================================================
#                   Copyright (C) 2000 by Niels Basjes
#                  Suck MT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.am
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : automake
#  $Date: 2000/04/02 21:18:55 $
#  $Revision: 1.24 $
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

if DEBUG
COMPILER_DEBUG = -O0 -g
#Keep the symbols in the final executables
LD_OMIT_SYMBOLS = 
else
COMPILER_DEBUG = -O3
#Remove the symbols from the final executables
LD_OMIT_SYMBOLS = -s
endif

CXXFLAGS = $(COMPILER_DEBUG)
CFLAGS = $(COMPILER_DEBUG)

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
EXTRA_DIST += SuckDefines.h config.h main.cpp tsqueue.h  
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
#ALLWARNINGS = -Wall -W  -Wtraditional -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual -Wsynth 
ALLWARNINGS = -Wall -W  -Wtraditional -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual -Wsynth 
CXXFLAGS += $(ALLWARNINGS)
CFLAGS += 

#Some flags for the regular expression library
CXXFLAGS += -DSTDC_HEADERS -DREGEX_MALLOC
CFLAGS   += -DSTDC_HEADERS -DREGEX_MALLOC

#------------------------------
# Flags for omnithread
# Linux ?
if UNAME_LINUX
OMNITHREADFLAGS = -D__x86__ -D__linux__ -D__OSVERSION__=2 -DNoNanoSleep -D_REENTRANT -DPthreadDraftVersion=10
PLATFORM=Linux
endif

# IRIX ?
# IRIX FLAGS TO BE CHECKED
if UNAME_IRIX
OMNITHREADFLAGS = -D__irix__ -DNoNanoSleep -D_REENTRANT -DPthreadDraftVersion=10
PLATFORM=Irix
endif

CXXFLAGS += $(OMNITHREADFLAGS)
CFLAGS += $(OMNITHREADFLAGS)

#------------------------------
# Explicit dependency
SuckDefines.h: VERSION
	touch SuckDefines.h

#------------------------------

cxxflags:
	@echo CXXFLAGS=$(CXXFLAGS)
	@echo PLATFORM=$(PLATFORM)


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

rpm: CopyrightMessage suckmt.lsm suckmt.spec dist
	rpm -ta $(distdir).tar.gz 
	mv /usr/src/*/RPMS/i386/suckmt*.rpm .
	mv /usr/src/*/SRPMS/suckmt*.rpm .

debug: CopyrightMessage configure
	./configure --enable-debug
	@echo The makefile has been created with debugging ENABLED
	@echo Run make again to actually start building suckmt.

nodebug: CopyrightMessage configure
	./configure --disable-debug
	@echo The makefile has been created with debugging DISABLED
	@echo Run make again to actually start building suckmt.

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

#End of the file Makefile.am
#=========================================================================
