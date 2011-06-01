# Microsoft Developer Studio Project File - Name="BuildQVM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=BuildQVM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BuildQVM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BuildQVM.mak" CFG="BuildQVM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BuildQVM - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "BuildQVM - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "BuildQVM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f BuildQVM.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "BuildQVM.exe"
# PROP BASE Bsc_Name "BuildQVM.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "buildvms.bat"
# PROP Rebuild_Opt ""
# PROP Target_File "..\baseef\game.qvm"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "BuildQVM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BuildQVM___Win32_Debug"
# PROP BASE Intermediate_Dir "BuildQVM___Win32_Debug"
# PROP BASE Cmd_Line "NMAKE /f BuildQVM.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "BuildQVM.exe"
# PROP BASE Bsc_Name "BuildQVM.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BuildQVM___Win32_Debug"
# PROP Intermediate_Dir "BuildQVM___Win32_Debug"
# PROP Cmd_Line "buildvms.bat"
# PROP Rebuild_Opt ""
# PROP Target_File "..\baseef\game.qvm"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "BuildQVM - Win32 Release"
# Name "BuildQVM - Win32 Debug"

!IF  "$(CFG)" == "BuildQVM - Win32 Release"

!ELSEIF  "$(CFG)" == "BuildQVM - Win32 Debug"

!ENDIF 

# Begin Source File

SOURCE=.\buildvms.bat

!IF  "$(CFG)" == "BuildQVM - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BuildQVM - Win32 Debug"

# PROP Intermediate_Dir "Debug"
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
