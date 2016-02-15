# Microsoft Developer Studio Project File - Name="mdslib_client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=mdslib_client - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mdslib_client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mdslib_client.mak" CFG="mdslib_client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mdslib_client - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "mdslib_client - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mdslib_client - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mdslib_c"
# PROP BASE Intermediate_Dir "mdslib_c"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "mdslib_client/Release"
# PROP Intermediate_Dir "mdslib_client/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_CLIENT_ONLY" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../Release/mdslib_client.dll"

!ELSEIF  "$(CFG)" == "mdslib_client - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mdslib_0"
# PROP BASE Intermediate_Dir "mdslib_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "mdslib_client/Debug"
# PROP Intermediate_Dir "mdslib_client/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_CLIENT_ONLY" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Debug/mdslib_client.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mdslib_client - Win32 Release"
# Name "mdslib_client - Win32 Debug"
# Begin Source File

SOURCE=..\mdstcpip\adler32.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\compress.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\deflate.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\infblock.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\infcodes.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\inffast.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\inflate.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\infutil.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\mdsip_socket_io.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\mdsiputil.c
# End Source File
# Begin Source File

SOURCE=.\MdsLib.c
# End Source File
# Begin Source File

SOURCE=.\MdsLib.def
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\trees.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\vmshpwd.c
# End Source File
# Begin Source File

SOURCE=..\mdstcpip\zutil.c
# End Source File
# End Target
# End Project
