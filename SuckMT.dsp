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
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "_WINSTATIC" /D "_MT" /YX /FD /c
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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "_WINSTATIC" /D "_MT" /FR /FD /GZ /c
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

SOURCE=.\main.cpp
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
# Begin Source File

SOURCE=.\nt.cpp
# End Source File
# Begin Source File

SOURCE=.\Printable.cpp
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
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Abortable.h
# End Source File
# Begin Source File

SOURCE=.\acconfig.h
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

SOURCE=.\GenericSocket.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
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

SOURCE=.\omnithread.h
# End Source File
# Begin Source File

SOURCE=.\Printable.h
# End Source File
# Begin Source File

SOURCE=.\StatisticsKeeper.h
# End Source File
# Begin Source File

SOURCE=.\SuckDefines.h
# End Source File
# Begin Source File

SOURCE=.\Tokenize.h
# End Source File
# Begin Source File

SOURCE=.\TraceLog.h
# End Source File
# Begin Source File

SOURCE=.\tsqueue.h
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
# End Group
# End Target
# End Project
