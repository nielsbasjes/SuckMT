# Microsoft Developer Studio Project File - Name="SuckMT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SuckMT - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SuckMT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SuckMT.mak" CFG="SuckMT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SuckMT - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SuckMT - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SuckMT"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SuckMT - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /D "NDEBUG" /D SUCKMT_VERSION=\"0.3j\" /D "STDC_HEADERS" /D "REGEX_MALLOC" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "_WINSTATIC" /D "_MT" /D "HAVE_CONFIG_H" /D "USE_BOOST_REGEX" /D "BOOST_RE_NO_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /I "./regex++" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "_WINSTATIC" /D "_MT" /D "HAVE_CONFIG_H" /D "USE_BOOST_REGEX" /D "BOOST_RE_NO_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /profile /debug /debugtype:both /machine:I386

!ENDIF 

# Begin Target

# Name "SuckMT - Win32 Release"
# Name "SuckMT - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "RegEx++"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\regex++\libs\regex\src\c_regex_traits.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\c_regex_traits_common.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\cpp_regex_traits.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\cregex.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\fileiter.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\posix_api.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\regex.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\regex_debug.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\regex_synch.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\w32_regex_traits.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\regex++\libs\regex\src\wide_posix_api.cpp"

!IF  "$(CFG)" == "SuckMT - Win32 Release"

!ELSEIF  "$(CFG)" == "SuckMT - Win32 Debug"

# ADD CPP /I "regex++"

!ENDIF 

# End Source File
# End Group
# Begin Group "SuckMT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ArticleImpactChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\DuplicatesChecker.cpp
# End Source File
# Begin Source File

SOURCE=.\HeaderMatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\NEWSArticle.cpp
# End Source File
# Begin Source File

SOURCE=.\NewsKiller.cpp
# End Source File
# Begin Source File

SOURCE=.\NNTPCommandHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\NNTPGetArticleCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\NNTPGetXOVERCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\NNTPProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\NNTPRetrieveManager.cpp
# End Source File
# End Group
# Begin Group "Generic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Abortable.cpp
# End Source File
# Begin Source File

SOURCE=.\AsciiLineSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Printable.cpp
# End Source File
# Begin Source File

SOURCE=.\SetProcTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\StatisticsKeeper.cpp
# End Source File
# Begin Source File

SOURCE=.\Tokenize.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceLog.cpp
# End Source File
# End Group
# Begin Group "omniThread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\nt.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Abortable.h
# End Source File
# Begin Source File

SOURCE=.\acconfig.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\alignment_traits.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\arithmetic_traits.hpp"
# End Source File
# Begin Source File

SOURCE=.\ArticleImpactChecker.h
# End Source File
# Begin Source File

SOURCE=.\AsciiLineSocket.h
# End Source File
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=.\CommandHandler.h
# End Source File
# Begin Source File

SOURCE=.\CommandQueue.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\composite_traits.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\config.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\conversion_traits.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\cregex.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\cv_traits.hpp"
# End Source File
# Begin Source File

SOURCE=.\DuplicatesChecker.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\fileiter.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\function_traits.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\fwd.hpp"
# End Source File
# Begin Source File

SOURCE=.\GenericSocket.h
# End Source File
# Begin Source File

SOURCE=.\HeaderMatcher.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\ice.hpp"
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\MultiStream.h
# End Source File
# Begin Source File

SOURCE=.\NEWSArticle.h
# End Source File
# Begin Source File

SOURCE=.\NewsKiller.h
# End Source File
# Begin Source File

SOURCE=.\NNTPCommandHandler.h
# End Source File
# Begin Source File

SOURCE=.\NNTPGetArticleCommand.h
# End Source File
# Begin Source File

SOURCE=.\NNTPGetXOVERCommand.h
# End Source File
# Begin Source File

SOURCE=.\NNTPProxy.h
# End Source File
# Begin Source File

SOURCE=.\NNTPRetrieveManager.h
# End Source File
# Begin Source File

SOURCE=.\omnithread\nt.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\object_traits.hpp"
# End Source File
# Begin Source File

SOURCE=.\omnithread.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\pattern_except.hpp"
# End Source File
# Begin Source File

SOURCE=.\Printable.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex.h"
# End Source File
# Begin Source File

SOURCE=.\regex.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_compile.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_config.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_cstring.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_format.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_kmp.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_library_include.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_match.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_options.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_raw_buffer.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_split.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_stack.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\detail\regex_synch.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\regex\regex_traits.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\same_traits.hpp"
# End Source File
# Begin Source File

SOURCE=.\SetProcTitle.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\smart_ptr.hpp"
# End Source File
# Begin Source File

SOURCE=.\StatisticsKeeper.h
# End Source File
# Begin Source File

SOURCE=.\SuckDefines.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\timer.hpp"
# End Source File
# Begin Source File

SOURCE=.\Tokenize.h
# End Source File
# Begin Source File

SOURCE=.\TraceLog.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\transform_traits.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\transform_traits_spec.hpp"
# End Source File
# Begin Source File

SOURCE=.\tsqueue.h
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\type_traits\type_traits_test.hpp"
# End Source File
# Begin Source File

SOURCE=".\regex++\boost\utility.hpp"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Other Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AUTHORS
# End Source File
# Begin Source File

SOURCE=.\ChangeLog
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=.\COPYING
# End Source File
# Begin Source File

SOURCE=.\INSTALL
# End Source File
# Begin Source File

SOURCE=.\NEWS
# End Source File
# Begin Source File

SOURCE=.\README
# End Source File
# Begin Source File

SOURCE=.\suckmt.ini.sample
# End Source File
# Begin Source File

SOURCE=.\suckmt.lsm
# End Source File
# Begin Source File

SOURCE=.\TODO
# End Source File
# Begin Source File

SOURCE=.\VERSION
# End Source File
# End Group
# End Target
# End Project
