bin_PROGRAMS = suckmt

suckmt_SOURCES = IniFile.cpp Abortable.cpp StatisticsKeeper.cpp stacktrace.cpp posix.cc main.cpp TraceLog.cpp Printable.cpp NewsKiller.cpp NNTPRetrieveManager.cpp NNTPProxy.cpp NNTPGetXOVERCommand.cpp NNTPGetArticleCommand.cpp NNTPCommandHandler.cpp NEWSArticle.cpp GenericSocket.cpp CommandQueue.cpp CommandHandler.cpp Command.cpp AsciiLineSocket.cpp Tokenize.cpp

suckmt_LDADD   = -lpthread

EXTRA_DIST = AsciiLineSocket.cpp AsciiLineSocket.h Command.cpp Command.h CommandHandler.cpp CommandHandler.h CommandQueue.cpp CommandQueue.h GenericSocket.cpp GenericSocket.h NEWSArticle.cpp NEWSArticle.h NNTPCommandHandler.cpp NNTPCommandHandler.h NNTPGetArticleCommand.cpp NNTPGetArticleCommand.h NNTPGetXOVERCommand.cpp NNTPGetXOVERCommand.h NNTPProxy.cpp NNTPProxy.h NNTPRetrieveManager.cpp NNTPRetrieveManager.h NewsKiller.cpp NewsKiller.h Printable.cpp Printable.h TraceLog.cpp TraceLog.h Vectorprint.h debugging.h main.cpp omnithread.h posix.cc stacktrace.cpp stacktrace.h tsqueue.h StatisticsKeeper.cpp StatisticsKeeper.h Abortable.cpp Abortable.h IniFile.cpp IniFile.h Tokenize.cpp Tokenize.h

# Optimization flags
#CXXFLAGS=-O0 -g3 
CXXFLAGS=-O2 

# Enable Compiler warnings
CXXFLAGS+= -Wall -W  -Wtraditional -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wsign-compare -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Woverloaded-virtual -Wsynth 

# Linux specific omnithread flags
CXXFLAGS+=-D__x86__ -D__linux__ -D__OSVERSION__=2 -DNoNanoSleep -D_REENTRANT -DPthreadDraftVersion=10

easydist: distclean
	rm configure
	cp Makefile.ed Makefile
