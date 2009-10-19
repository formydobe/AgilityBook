# Microsoft Developer Studio Project File - Name="AgilityBook" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AgilityBook - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AgilityBook.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AgilityBook.mak" CFG="AgilityBook - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AgilityBook - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "AgilityBook - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "AgilityBook"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AgilityBook - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../bin/VC6/Debug"
# PROP Intermediate_Dir "../../../bin/VC6/Debug/AgilityBook"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../.." /I "../../Include" /I "../../ARB" /I "../../Win" /I "../../../../boost" /I "$(WXWIN)/include" /I "$(WXWIN)/include/msvc" /D "WXWIDGETS" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_WINDLL" /D "WXUSINGDLL" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../../Include" /i "$(WXWIN)/include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib comctl32.lib rpcrt4.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../../lib/VC6" /libpath:"$(WXWIN)\lib\vc_dll_VC60"

!ELSEIF  "$(CFG)" == "AgilityBook - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../bin/VC6/Release"
# PROP Intermediate_Dir "../../../bin/VC6/Release/AgilityBook"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../.." /I "../../Include" /I "../../ARB" /I "../../Win" /I "../../../../boost" /I "$(WXWIN)/include" /I "$(WXWIN)/include/msvc" /D "WXWIDGETS" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_WINDLL" /D "WXUSINGDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "../../Include" /i "$(WXWIN)/include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib comctl32.lib rpcrt4.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /libpath:"../../../lib/VC6" /libpath:"$(WXWIN)\lib\vc_dll_VC60"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "AgilityBook - Win32 Debug"
# Name "AgilityBook - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\Win\AgilityBook.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookCalendarListView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookCalendarView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookDoc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookHtmlView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookOptions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookPanels.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookPointsView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookRunsView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookTrainingView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookTreeData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookTreeView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\CalendarSites.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\CheckLink.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\CheckTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\ClipBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\ColumnOrder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\ComboBoxes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\CommonView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\ConfigHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgAbout.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgAssignColumns.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgAuthenticate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgCalendar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgCalendarQueryDetail.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgClub.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigEventMethod.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigMultiQ.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigOtherPoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigPlaceInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigTitle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigTitlePoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigUpdate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigure.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigureData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigVenue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgCRCDViewer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgDog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgEventSelect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgExistingPoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgFault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgFind.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgFindLinks.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgInfoNote.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgListViewer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptionsCalendar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptionsFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptionsPrint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptionsProgram.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOtherPoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgPartner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgPluginDetails.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgPointsViewSort.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgProgress.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgQueryDetail.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgReferenceRun.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRegNum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgReorder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRun.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgSelectDog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgSelectURL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgTitle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgTraining.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgTrial.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\FilterOptions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\Globals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\IconList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\LanguageManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\ListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\ListData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\Localization.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\NoteButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\PointsData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\Print.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\ReadHttp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\RichEditCtrl2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\Win\TabView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\UpdateInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\Validators.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\Widgets.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\Wizard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\WizardExcel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\WizardExport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\WizardImport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Win\WizardStart.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Win\AgilityBook.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookCalendarListView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookCalendarView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookDoc.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookHtmlView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookOptions.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookPanels.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookPointsView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookRunsView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookTrainingView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookTreeData.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\AgilityBookTreeView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\CalendarSites.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\CheckLink.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\CheckTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\ClipBoard.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\ColumnOrder.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\ComboBoxes.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\CommonView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\ConfigHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgAbout.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgAssignColumns.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgAuthenticate.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgCalendar.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgCalendarQueryDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgClub.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigEventMethod.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigMultiQ.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigOtherPoints.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigPlaceInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigTitle.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigTitlePoints.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigUpdate.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigure.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigureData.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgConfigVenue.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgCRCDViewer.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgDog.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgEventSelect.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgExistingPoints.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgFault.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgFind.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgFindLinks.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgInfoNote.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgListViewer.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgMessageBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgName.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptions.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptionsCalendar.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptionsFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptionsPrint.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOptionsProgram.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgOtherPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgPartner.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgPluginDetails.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgPointsViewSort.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgProgress.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgQueryDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgReferenceRun.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRegNum.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgReorder.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRun.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRunComments.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRunCRCD.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRunLink.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRunReference.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgRunScore.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgSelectDog.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgSelectURL.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgTitle.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgTraining.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\DlgTrial.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\excel8.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\FilterOptions.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\Globals.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\IconList.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\LanguageManager.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\ListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\ListData.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\Localization.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\NoteButton.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\ooCalc.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\PointsData.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\ReadHttp.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\RichEditCtrl2.h
# End Source File
# Begin Source File

SOURCE=..\..\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\TabView.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\UpdateInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\Validators.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\Widgets.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\Wizard.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\WizardExcel.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\WizardExport.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\WizardImport.h
# End Source File
# Begin Source File

SOURCE=..\..\Win\WizardStart.h
# End Source File
# End Group
# Begin Group "ARB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ARB\ARBAgilityRecordBook.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBAgilityRecordBook.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBBase.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBBase64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBBase64.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBCalendar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBCalendar.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigAction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigAction.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigCalSite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigCalSite.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigDivision.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigDivision.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigFault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigFault.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigLevel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigLevel.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigLifetimePoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigLifetimePoints.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigMultiQ.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigMultiQ.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigOtherPoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigOtherPoints.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigPlaceInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigPlaceInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigScoring.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigScoring.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigSubLevel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigSubLevel.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigTitle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigTitle.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigTitlePoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigTitlePoints.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigVenue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBConfigVenue.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDate.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDog.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogClub.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogClub.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogExistingPoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogExistingPoints.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogNotes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogNotes.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogReferenceRun.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogReferenceRun.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRegNum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRegNum.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRun.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRun.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRunOtherPoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRunOtherPoints.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRunPartner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRunPartner.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRunScoring.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogRunScoring.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogTitle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogTitle.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogTrial.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBDogTrial.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBInfoItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBInfoItem.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBLocalization.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBLocalization.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBString.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBTraining.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBTraining.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\ARBTypes.h
# End Source File
# Begin Source File

SOURCE=..\..\ARB\BinaryData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ARB\BinaryData.h
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
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Win\res\AgilityBook.ico
# End Source File
# Begin Source File

SOURCE=..\..\Win\res\l.enu\AgilityBook.rc2
# End Source File
# Begin Source File

SOURCE=..\..\Win\res\AgilityRecordBook.dtd
# End Source File
# Begin Source File

SOURCE=..\..\Win\res\DefaultConfig.xml
# End Source File
# End Group
# End Target
# End Project
# Section AgilityBook : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:106
# 	2:21:SplashScreenInsertKey:4.0
# End Section