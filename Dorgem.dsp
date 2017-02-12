# Microsoft Developer Studio Project File - Name="Dorgem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Dorgem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Dorgem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Dorgem.mak" CFG="Dorgem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Dorgem - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Dorgem - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Dorgem - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /I "C:\Program Files\HTML Help Workshop\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x413 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 msimg32.lib "C:\Program Files\HTML Help Workshop\LIB\HTMLHELP.LIB" /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc"

!ELSEIF  "$(CFG)" == "Dorgem - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"StdAfx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /ZI /Od /I "C:\Program Files\HTML Help Workshop\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"StdAfx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x413 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msimg32.lib "C:\Program Files\HTML Help Workshop\LIB\HTMLHELP.LIB" /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Dorgem - Win32 Release"
# Name "Dorgem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionBitmapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionPosDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionText.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptionTextDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdLine.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Dorgem.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlHelp\Dorgem.hhp

!IF  "$(CFG)" == "Dorgem - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
InputPath=.\HtmlHelp\Dorgem.hhp
InputName=Dorgem

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\MakeHelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "Dorgem - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
InputPath=.\HtmlHelp\Dorgem.hhp
InputName=Dorgem

"$(OutDir)\$(InputName).chm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\MakeHelp.bat"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dorgem.rc
# End Source File
# Begin Source File

SOURCE=.\DorgemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DorgemEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DunManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\FtpClient.cpp
# End Source File
# Begin Source File

SOURCE=.\Help.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\HelpPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\HookWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\JpegFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ListenSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\NTray.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsAviPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsGeneralPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsWebServerPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegistryEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\RegKey.cpp
# End Source File
# Begin Source File

SOURCE=.\Request.cpp
# End Source File
# Begin Source File

SOURCE=.\RequestSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\SInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# Begin Source File

SOURCE=.\StoreEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreFTP.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreFTPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreHTTP.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreHTTPDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreProgramDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreSettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tm.cpp
# End Source File
# Begin Source File

SOURCE=.\VFWWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\WriteAVI.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\CaptionBitmap.h
# End Source File
# Begin Source File

SOURCE=.\CaptionBitmapDlg.h
# End Source File
# Begin Source File

SOURCE=.\CaptionEvent.h
# End Source File
# Begin Source File

SOURCE=.\CaptionManager.h
# End Source File
# Begin Source File

SOURCE=.\CaptionPosDlg.h
# End Source File
# Begin Source File

SOURCE=.\CaptionSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\CaptionSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\CaptionText.h
# End Source File
# Begin Source File

SOURCE=.\CaptionTextDlg.h
# End Source File
# Begin Source File

SOURCE=.\CmdLine.h
# End Source File
# Begin Source File

SOURCE=.\ColorSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorStatic.h
# End Source File
# Begin Source File

SOURCE=.\Dorgem.h
# End Source File
# Begin Source File

SOURCE=.\DorgemDlg.h
# End Source File
# Begin Source File

SOURCE=.\DorgemEvent.h
# End Source File
# Begin Source File

SOURCE=.\DunManager.h
# End Source File
# Begin Source File

SOURCE=.\EventManager.h
# End Source File
# Begin Source File

SOURCE=.\FtpClient.h
# End Source File
# Begin Source File

SOURCE=.\Help.h
# End Source File
# Begin Source File

SOURCE=.\HelpDialog.h
# End Source File
# Begin Source File

SOURCE=.\HelpPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\HookWnd.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\JpegFile.h
# End Source File
# Begin Source File

SOURCE=.\ListenSocket.h
# End Source File
# Begin Source File

SOURCE=.\NTray.h
# End Source File
# Begin Source File

SOURCE=.\OptionsAviPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionsGeneralPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsWebServerPage.h
# End Source File
# Begin Source File

SOURCE=.\PreviewDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegistryEntry.h
# End Source File
# Begin Source File

SOURCE=.\RegKey.h
# End Source File
# Begin Source File

SOURCE=.\Request.h
# End Source File
# Begin Source File

SOURCE=.\RequestSocket.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Resource.hm
# End Source File
# Begin Source File

SOURCE=.\SInstance.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StoreEvent.h
# End Source File
# Begin Source File

SOURCE=.\StoreFile.h
# End Source File
# Begin Source File

SOURCE=.\StoreFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\StoreFTP.h
# End Source File
# Begin Source File

SOURCE=.\StoreFTPDlg.h
# End Source File
# Begin Source File

SOURCE=.\StoreHTTP.h
# End Source File
# Begin Source File

SOURCE=.\StoreHTTPDlg.h
# End Source File
# Begin Source File

SOURCE=.\StoreManager.h
# End Source File
# Begin Source File

SOURCE=.\StoreProgram.h
# End Source File
# Begin Source File

SOURCE=.\StoreProgramDlg.h
# End Source File
# Begin Source File

SOURCE=.\StoreSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\StoreSettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tm.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# Begin Source File

SOURCE=.\VFWWnd.h
# End Source File
# Begin Source File

SOURCE=.\WriteAVI.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\ColorSelect.cur
# End Source File
# Begin Source File

SOURCE=.\res\Dorgem.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Dorgem.manifest
# End Source File
# Begin Source File

SOURCE=.\res\Dorgem.rc2
# End Source File
# Begin Source File

SOURCE=.\Res\LinkCursor.cur
# End Source File
# End Group
# End Target
# End Project
