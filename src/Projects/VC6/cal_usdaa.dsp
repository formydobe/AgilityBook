# Microsoft Developer Studio Project File - Name="cal_usdaa" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cal_usdaa - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cal_usdaa.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cal_usdaa.mak" CFG="cal_usdaa - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cal_usdaa - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cal_usdaa - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cal_usdaa - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../bin/VC6/Debug"
# PROP Intermediate_Dir "../../../bin/VC6/Debug/cal_usdaa"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../.." /I "../../Include" /I "../../ARB" /I "../../cal_usdaa" /I "../../../../boost" /I "$(WXWIN)/include" /I "$(WXWIN)/include/msvc" /D "WXWIDGETS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_WINDLL" /D "WXUSINGDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../../Include" /i "$(WXWIN)/include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib comctl32.lib rpcrt4.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"$(WXWIN)\lib\vc_dll_VC60"

!ELSEIF  "$(CFG)" == "cal_usdaa - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../bin/VC6/Release"
# PROP Intermediate_Dir "../../../bin/VC6/Release/cal_usdaa"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../.." /I "../../Include" /I "../../ARB" /I "../../cal_usdaa" /I "../../../../boost" /I "$(WXWIN)/include" /I "$(WXWIN)/include/msvc" /D "WXWIDGETS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_WINDLL" /D "WXUSINGDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "../../Include" /i "$(WXWIN)/include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ws2_32.lib comctl32.lib rpcrt4.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"$(WXWIN)\lib\vc_dll_VC60"

!ENDIF 

# Begin Target

# Name "cal_usdaa - Win32 Debug"
# Name "cal_usdaa - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\cal_usdaa\cal_usdaa.cpp
# End Source File
# Begin Source File

SOURCE=..\..\cal_usdaa\cal_usdaa.def
# End Source File
# Begin Source File

SOURCE=..\..\cal_usdaa\cal_usdaa.rc
# End Source File
# Begin Source File

SOURCE=..\..\cal_usdaa\CalendarSite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\cal_usdaa\res\cal_usdaa.rc2
# End Source File
# End Group
# Begin Group "ARB Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ARB\ARBDate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDate.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBString.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\Element.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\Element.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\VersionNum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\VersionNum.h
# End Source File
# End Group
# End Target
# End Project