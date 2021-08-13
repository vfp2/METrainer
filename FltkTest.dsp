# Microsoft Developer Studio Project File - Name="FltkTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=FltkTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FltkTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FltkTest.mak" CFG="FltkTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FltkTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "FltkTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FltkTest - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /Zm300 /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 shfolder.lib Shell32.lib fltk.lib opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib ole32.lib oleaut32.lib winmm.lib fltkimages.lib fltkjpeg.lib fltkpng.lib fltkz.lib FTD2XX.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "FltkTest - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /Zm300 /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 SHFolder.lib shell32.lib fltk.lib opengl32.lib wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib ole32.lib oleaut32.lib winmm.lib fltkimages.lib fltkjpeg.lib fltkpng.lib fltkz.lib FTD2XX.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libcmt" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "FltkTest - Win32 Release"
# Name "FltkTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Fl_Shaped_Window.cpp
# End Source File
# Begin Source File

SOURCE=.\FtdiRng.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiCommandLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\IPrd.cpp
# End Source File
# Begin Source File

SOURCE=.\IRng.cpp
# End Source File
# Begin Source File

SOURCE=.\LfsrCorrector.cpp
# End Source File
# Begin Source File

SOURCE=.\LpFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\MersenneTwister.cpp
# End Source File
# Begin Source File

SOURCE=.\PcqngAnnPrd.cpp
# End Source File
# Begin Source File

SOURCE=.\PcqngPrd.cpp
# End Source File
# Begin Source File

SOURCE=.\PcqngRng.cpp
# End Source File
# Begin Source File

SOURCE=.\PrdSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\PsiCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\PsiLogger.c
# End Source File
# Begin Source File

SOURCE=.\PsiSeerApp.cpp
# End Source File
# Begin Source File

SOURCE=.\PsiSeerGui.cpp
# End Source File
# Begin Source File

SOURCE=.\PsiSeerGuiHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\PsiSeerWinRc.rc
# End Source File
# Begin Source File

SOURCE=.\QwPrd4.cpp
# End Source File
# Begin Source File

SOURCE=.\RngBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionPlot.cpp
# End Source File
# Begin Source File

SOURCE=.\StatsGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\StatsKeeper.cpp
# End Source File
# Begin Source File

SOURCE=.\Types.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BoundedRandomWalk.hpp
# End Source File
# Begin Source File

SOURCE=.\Fl_Shaped_Window.H
# End Source File
# Begin Source File

SOURCE=.\FtdiRng.hpp
# End Source File
# Begin Source File

SOURCE=.\GuiCommandLayer.hpp
# End Source File
# Begin Source File

SOURCE=.\IBuffer.hpp
# End Source File
# Begin Source File

SOURCE=.\IPrd.hpp
# End Source File
# Begin Source File

SOURCE=.\IRng.hpp
# End Source File
# Begin Source File

SOURCE=.\LfsrCorrector.hpp
# End Source File
# Begin Source File

SOURCE=.\LineChart.hpp
# End Source File
# Begin Source File

SOURCE=.\LpFilter.hpp
# End Source File
# Begin Source File

SOURCE=.\MersenneTwister.hpp
# End Source File
# Begin Source File

SOURCE=.\PcqngAnnPrd.hpp
# End Source File
# Begin Source File

SOURCE=.\PcqngPrd.hpp
# End Source File
# Begin Source File

SOURCE=.\PcqngRng.hpp
# End Source File
# Begin Source File

SOURCE=.\PrdSelector.hpp
# End Source File
# Begin Source File

SOURCE=.\PsiLogger.h
# End Source File
# Begin Source File

SOURCE=.\PsiSeerApp.hpp
# End Source File
# Begin Source File

SOURCE=.\PsiSeerGui.hpp
# End Source File
# Begin Source File

SOURCE=.\PsiSeerGuiHelp.hpp
# End Source File
# Begin Source File

SOURCE=.\psistats.h
# End Source File
# Begin Source File

SOURCE=.\QwPrd4.hpp
# End Source File
# Begin Source File

SOURCE=.\RngBuffer.hpp
# End Source File
# Begin Source File

SOURCE=.\SessionPlot.hpp
# End Source File
# Begin Source File

SOURCE=.\StatsGroup.hpp
# End Source File
# Begin Source File

SOURCE=.\StatsKeeper.hpp
# End Source File
# Begin Source File

SOURCE=.\Types.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# End Target
# End Project
