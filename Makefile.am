#=========================================================================
#                   Copyright (C) 2000 by Niels Basjes
#                  SuckMT Website: http://go.to/suckmt
#                        Author: SuckMT@Basjes.nl
#-------------------------------------------------------------------------
#  Filename  : Makefile.am
#  Sub-system: SuckMT, a multithreaded suck replacement
#  Language  : automake
#  $Date: 2001/08/27 20:41:31 $
#  $Revision: 1.36 $
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

DOCS=README ChangeLog INSTALL suckmt.lsm suckmt.ini.sample AUTHORS COPYING README NEWS
datadir=${prefix}/var/spool/suckmt
docdir=${prefix}/usr/doc/packages/suckmt
configdir=${prefix}/etc
bindir=${prefix}/usr/bin
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
SUCKSOURCES += Printable.cpp 
SUCKSOURCES += StatisticsKeeper.cpp 
SUCKSOURCES += Tokenize.cpp
SUCKSOURCES += MultiStream.cpp 
SUCKSOURCES += TraceLog.cpp 

SUCKSOURCES += regex.c
#SUCKSOURCES += c_regex_traits.cpp
#SUCKSOURCES += c_regex_traits_common.cpp
#SUCKSOURCES += cpp_regex_traits.cpp
#SUCKSOURCES += cregex.cpp
#SUCKSOURCES += fileiter.cpp
#SUCKSOURCES += posix_api.cpp
#SUCKSOURCES += regex.cpp
#SUCKSOURCES += regex_debug.cpp
#SUCKSOURCES += regex_synch.cpp
#SUCKSOURCES += w32_regex_traits.cpp
#SUCKSOURCES += wide_posix_api.cpp

suckmt_SOURCES = $(SUCKSOURCES)
suckmtstatic_SOURCES = $(SUCKSOURCES)

suckmt_LDADD          = -lpthread 
suckmt_LDFLAGS        = $(LD_OMIT_SYMBOLS)
suckmtstatic_LDADD    = -lpthread
suckmtstatic_LDFLAGS  = $(LD_OMIT_SYMBOLS) -static

# Extradist includes some generic files
EXTRA_DIST  = Makefile.ed suckmt.ini.sample.in suckmt.ini.sample
EXTRA_DIST += configure.in_ VERSION
EXTRA_DIST += suckmt.lsm.in  suckmt.lsm 
EXTRA_DIST += suckmt.spec.in suckmt.spec

# The MS-Visual Studio Specific Project/Make files.
EXTRA_DIST += SuckMT.dsp SuckMT.dsw 

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

# The regex++ sources for all platforms
EXTRA_DIST += regex++/boost/regex/detail/fileiter.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_compile.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_config.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_cstring.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_format.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_kmp.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_library_include.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_match.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_options.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_raw_buffer.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_split.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_stack.hpp
EXTRA_DIST += regex++/boost/regex/detail/regex_synch.hpp
EXTRA_DIST += regex++/boost/regex/pattern_except.hpp
EXTRA_DIST += regex++/boost/regex/regex_traits.hpp
EXTRA_DIST += regex++/boost/config.hpp
EXTRA_DIST += regex++/boost/cregex.hpp
EXTRA_DIST += regex++/boost/regex.h
EXTRA_DIST += regex++/boost/regex.hpp
EXTRA_DIST += regex++/boost/smart_ptr.hpp
EXTRA_DIST += regex++/boost/timer.hpp
EXTRA_DIST += regex++/boost/type_traits.hpp
EXTRA_DIST += regex++/boost/utility.hpp
EXTRA_DIST += regex++/boost/type_traits/alignment_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/arithmetic_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/composite_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/conversion_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/cv_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/function_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/fwd.hpp
EXTRA_DIST += regex++/boost/type_traits/ice.hpp
EXTRA_DIST += regex++/boost/type_traits/object_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/same_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/transform_traits.hpp
EXTRA_DIST += regex++/boost/type_traits/transform_traits_spec.hpp
EXTRA_DIST += regex++/boost/type_traits/type_traits_test.hpp
EXTRA_DIST += regex++/c++boost.gif
EXTRA_DIST += regex++/regex.htm
EXTRA_DIST += regex++/libs/regex/src/c_regex_traits.cpp
EXTRA_DIST += c_regex_traits.cpp
EXTRA_DIST += regex++/libs/regex/src/c_regex_traits_common.cpp
EXTRA_DIST += c_regex_traits_common.cpp
EXTRA_DIST += regex++/libs/regex/src/cpp_regex_traits.cpp
EXTRA_DIST += cpp_regex_traits.cpp
EXTRA_DIST += regex++/libs/regex/src/cregex.cpp
EXTRA_DIST += cregex.cpp
EXTRA_DIST += regex++/libs/regex/src/fileiter.cpp
EXTRA_DIST += fileiter.cpp
EXTRA_DIST += regex++/libs/regex/src/posix_api.cpp
EXTRA_DIST += posix_api.cpp
EXTRA_DIST += regex++/libs/regex/src/primary_transform.hpp
EXTRA_DIST += primary_transform.hpp
EXTRA_DIST += regex++/libs/regex/src/regex.cpp
EXTRA_DIST += regex.cpp
EXTRA_DIST += regex++/libs/regex/src/regex_debug.cpp
EXTRA_DIST += regex_debug.cpp
EXTRA_DIST += regex++/libs/regex/src/regex_synch.cpp
EXTRA_DIST += regex_synch.cpp
EXTRA_DIST += regex++/libs/regex/src/w32_regex_traits.cpp
EXTRA_DIST += w32_regex_traits.cpp
EXTRA_DIST += regex++/libs/regex/src/wide_posix_api.cpp
EXTRA_DIST += wide_posix_api.cpp
EXTRA_DIST += regex++/libs/regex/appendix.htm
EXTRA_DIST += regex++/libs/regex/changes.txt
EXTRA_DIST += regex++/libs/regex/dmi.sh
EXTRA_DIST += regex++/libs/regex/faq.htm
EXTRA_DIST += regex++/libs/regex/format_string.htm
EXTRA_DIST += regex++/libs/regex/hl_ref.htm
EXTRA_DIST += regex++/libs/regex/index.htm
EXTRA_DIST += regex++/libs/regex/introduction.htm
EXTRA_DIST += regex++/libs/regex/licence.txt
EXTRA_DIST += regex++/libs/regex/posix_ref.htm
EXTRA_DIST += regex++/libs/regex/readme.txt
EXTRA_DIST += regex++/libs/regex/syntax.htm
EXTRA_DIST += regex++/libs/regex/template_class_ref.htm
EXTRA_DIST += regex++/libs/regex/traits_class_ref.htm

# Enable Compiler warnings
#ALLWARNINGS = -Wall -W  -Wtraditional -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual -Wsynth 
ALLWARNINGS = -Wall -W  -Wtraditional -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual -Wsynth 
CXXFLAGS += $(ALLWARNINGS)
CFLAGS += 

#Some flags for the regular expression library
#CXXFLAGS += -I ./regex++ -DBOOST_RE_NO_LIB
#CFLAGS   += -I ./regex++ -DBOOST_RE_NO_LIB

#Some flags for the regular expression library
CXXFLAGS += -DSTDC_HEADERS -DREGEX_MALLOC
CFLAGS   += -DSTDC_HEADERS -DREGEX_MALLOC

#------------------------------
# Platform dependend flags for omnithread
# Linux ?
if UNAME_LINUX
OMNITHREADFLAGS = -D__x86__ -D__linux__ -D__OSVERSION__=2 -DNoNanoSleep -D_REENTRANT -DPthreadDraftVersion=10
PLATFORM=Linux
endif

# IRIX ?
if UNAME_IRIX
OMNITHREADFLAGS = -D__mips__ -D__irix__ -D__OSVERSION__=6  -DUsePthread -D_REENTRANT -DPthreadDraftVersion=10 -DPthreadSupportThreadPriority
PLATFORM=Irix
endif

# IRIX 64 bits ?
if UNAME_IRIX64
OMNITHREADFLAGS = -D__mips__ -D__irix__ -D__OSVERSION__=6  -DUsePthread -D_REENTRANT -DPthreadDraftVersion=10 -DPthreadSupportThreadPriority
PLATFORM=Irix64
endif

# Solaris ?
if UNAME_SOLARIS
OMNITHREADFLAGS = -D__sunos__ -D__sparc__ -D__OSVERSION__=5 -DSVR4 -DUsePthread -D_REENTRANT -DPthreadDraftVersion=10
PLATFORM=Solaris
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

full-distribution-set: all rpm bin-dist bin-static-dist bin-win32-dist
	
bin-dist: all suckmt.ini.sample
	rm -rf suckmt-`cat VERSION`
	mkdir suckmt-`cat VERSION`
	cp suckmt.ini.sample $(DOCS) suckmt-`cat VERSION`
	cp suckmt suckmt-`cat VERSION`/suckmt
	tar czf suckmt-`cat VERSION`-bin-`uname --sysname`-`uname --machine`.tar.gz suckmt-`cat VERSION`
	rm -rf suckmt-`cat VERSION`

bin-static-dist: all suckmtstatic suckmt.ini.sample
	rm -rf suckmt-`cat VERSION`
	mkdir suckmt-`cat VERSION`
	cp suckmt.ini.sample $(DOCS) suckmt-`cat VERSION`
	cp suckmtstatic suckmt-`cat VERSION`/suckmt
	tar czf suckmt-`cat VERSION`-static-bin-`uname --sysname`-`uname --machine`.tar.gz suckmt-`cat VERSION`
	rm -rf suckmt-`cat VERSION`

bin-win32-dist: SuckMT.exe suckmt.ini.sample.win32
	rm -rf suckmt-`cat VERSION` 
	mkdir suckmt-`cat VERSION` 
	cp $(DOCS) suckmt-`cat VERSION` 
	cp suckmt.ini.sample.win32 suckmt-`cat VERSION`/suckmt.ini.sample 
	cp SuckMT.exe suckmt-`cat VERSION` 
	zip suckmt-`cat VERSION`-win32.zip suckmt-`cat VERSION`/*
	rm -rf suckmt-`cat VERSION` 

# This target makes the distribution easier to build for the end user.
dist-hook: suckmt.lsm suckmt.spec suckmt.ini.sample
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

suckmt.ini.sample: suckmt.ini.sample.in configure.in suckmt 
	cp -f suckmt.ini.sample.in suckmt.ini.sample
	./suckmt -init -i ./suckmt.ini.sample

suckmt.ini.sample.win32:suckmt.ini.sample
	sed "s/\/var\/spool\/suckmt\//C:\\\\TEMP\\\\SuckMT\\\\/g" < suckmt.ini.sample \
        | sed "s/\/tmp\//C:\\\\TEMP\\\\SuckMT\\\\/g" > suckmt.ini.sample.win32

install: rpminstall 
	if [ -s $(configdir)/suckmt.ini ];
	then
	    NOW=`date`
	    cp -f $(configdir)/suckmt.ini "$(configdir)/suckmt.ini-backup-${NOW}"
	    echo Your original $(configdir)/suckmt.ini has been backed up as "$(configdir)/suckmt.ini-backup-${NOW}"
	fi
	suckmt -init -i $(configdir)/suckmt.ini

rpminstall: all suckmt.ini.sample
	@echo "Creating directories..."
	$(INSTALL) -d $(bindir) $(datadir) $(datadir)/in.coming $(docdir) 
	@echo "Installing $(bin_PROGRAMS) executable..."
	$(INSTALL_PROGRAM) $(bin_PROGRAMS) $(bindir)/$(bin_PROGRAMS)
	@echo "Creating default configuration file..."
	[ -d $(configdir) ] || $(INSTALL) -d $(configdir)
	touch $(configdir)/suckmt.ini
	@echo "Installing documentation..."
	#$(INSTALL_DATA) suckmt.1 $(man1dir)/suckmt.1
	for f in $(DOCS); do $(INSTALL_DATA) $$f $(docdir)/$$f; done

rpm: CopyrightMessage suckmt.lsm suckmt.spec suckmt.ini.sample dist 
	rpm -ta $(distdir).tar.gz  
	mv `rpm --showrc | grep ^rpmdir | awk  '{print$$3}'`/*/$(distdir)*.rpm .
	mv `rpm --showrc | grep ^srcrpmdir | awk  '{print$$3}'`/$(distdir)*.rpm .

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
	@echo "| SuckMT `cat VERSION` - A Multi Threaded suck replacement. |"
	@echo "| (C)2001 by Niels Basjes  -  http://go.to/suckmt  |"
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

full-revert: clean
	rm -f configure configure.in config.status config.log config.h 
	cp Makefile.bootstrap Makefile

#End of the file Makefile.am
#=========================================================================
