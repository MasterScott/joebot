# Microsoft Developer Studio Project File - Name="JoeBot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JoeBot - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JoeBot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JoeBot.mak" CFG="JoeBot - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JoeBot - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JoeBot - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SDKSrc/Public/dlls", NVGBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JoeBot - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /MT /W3 /GX /Zi /O2 /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /I "../NNSim/" /I "../NNSim/SOM/" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VALVE_DLL" /D "_WIN32" /FA /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /def:".\JoeBot.def"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
ProjDir=.
TargetPath=.\Release\JoeBot.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Compressing and copying DLL
PostBuild_Cmds=copy                                   $(TargetPath)                                   c:\spiele\half-life\joebot	copy                                   $(TargetPath)                                   h:\CJoeBot\joebot	copy                                    $(ProjDir)\..\docs\versions.txt                          h:\CJoeBot\joebot\       	copy                                    $(ProjDir)\..\docs\readme.html                          h:\CJoeBot\joebot	copy                                   $(TargetPath)                                   m:\half-life\joebot
# End Special Build Tool

!ELSEIF  "$(CFG)" == "JoeBot - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\JoeBot___Win"
# PROP BASE Intermediate_Dir ".\JoeBot___Win"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MTd /W3 /GR /GX /ZI /Od /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\\" /I "../NNSim/" /I "../NNSim/SOM/" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "VALVE_DLL" /FAs /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\engine" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib advapi32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\Joebot.def"
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
ProjDir=.
TargetPath=.\Debug\JoeBot.dll
SOURCE="$(InputPath)"
PostBuild_Cmds=copy                                   $(TargetPath)                                   c:\spiele\half-life\joebot	copy                                   $(TargetPath)                                   h:\CJoeBot\joebot	copy                                    $(ProjDir)\..\docs\versions.txt                          h:\CJoeBot\joebot       	copy                                    $(ProjDir)\..\docs\readme.html                          h:\CJoeBot\joebot	copy                                   $(TargetPath)                                   m:\half-life\joebot
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "JoeBot - Win32 Release"
# Name "JoeBot - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\NNSim\Activations.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseLink.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseLinkFF.cpp
# End Source File
# Begin Source File

SOURCE=.\basemem.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseNeuralNet.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseNeuralNetFF.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseNeuron.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseNeuronFF.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BasePattern.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BasePatternElem.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BasePatternElemFF.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BasePatternFF.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseWeight.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseWeightFF.cpp
# End Source File
# Begin Source File

SOURCE=.\bot.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_client.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_cs.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_names.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_wpdir.cpp
# End Source File
# Begin Source File

SOURCE=.\bot_wpstat.cpp
# End Source File
# Begin Source File

SOURCE=.\CBotBase.cpp
# End Source File
# Begin Source File

SOURCE=.\CBotCS.cpp
# End Source File
# Begin Source File

SOURCE=.\CBotCS_combat.cpp
# End Source File
# Begin Source File

SOURCE=.\CBotCS_navigate.cpp
# End Source File
# Begin Source File

SOURCE=.\CBotDOD.cpp
# End Source File
# Begin Source File

SOURCE=.\CBotDOD_combat.cpp
# End Source File
# Begin Source File

SOURCE=.\CBotDOD_navigate.cpp
# End Source File
# Begin Source File

SOURCE=.\CCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\Chat.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatBot.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatHost.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatHostData.cpp
# End Source File
# Begin Source File

SOURCE=.\CLTMem.cpp
# End Source File
# Begin Source File

SOURCE=.\Commandfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\CParser.cpp
# End Source File
# Begin Source File

SOURCE=.\CPersonality.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\CqSOM2d.cpp
# End Source File
# Begin Source File

SOURCE=.\CRadio.cpp
# End Source File
# Begin Source File

SOURCE=.\CSDecals.cpp
# End Source File
# Begin Source File

SOURCE=.\CSenses.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CState.cpp
# End Source File
# Begin Source File

SOURCE=.\CSTMem.cpp
# End Source File
# Begin Source File

SOURCE=.\CTask.cpp
# End Source File
# Begin Source File

SOURCE=.\dll.cpp
# End Source File
# Begin Source File

SOURCE=.\engine.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\fNBorKernel.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\Genom.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\GenomToken.cpp
# End Source File
# Begin Source File

SOURCE=.\Gnome.cpp
# End Source File
# Begin Source File

SOURCE=.\h_export.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\LinkBProp.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\LinkBPropM.cpp
# End Source File
# Begin Source File

SOURCE=.\linkfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\LListElem.cpp
# End Source File
# Begin Source File

SOURCE=.\namefunc.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuralNetBProp.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuralNetBPropM.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuronBProp.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuronBPropM.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\NNSimService.cpp
# End Source File
# Begin Source File

SOURCE=.\NNWeapon.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\nVec.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\PatternBProp.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\PatternElemBProp.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\SOMPattern.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\SOMPatternElem.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\waypoint.cpp
# End Source File
# Begin Source File

SOURCE=.\waypoint_advdataimpl.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\WeightBProp.cpp
# End Source File
# Begin Source File

SOURCE=..\NNSim\WeightBPropM.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldGnome.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\NNSim\Activations.h
# End Source File
# Begin Source File

SOURCE=.\activity.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseLink.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseLinkFF.h
# End Source File
# Begin Source File

SOURCE=.\basemem.h
# End Source File
# Begin Source File

SOURCE=.\basemonster.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseNeuralNet.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseNeuralNetFF.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseNeuron.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseNeuronFF.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BasePattern.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BasePatternElem.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BasePatternElemFF.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BasePatternFF.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseWeight.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BaseWeightFF.h
# End Source File
# Begin Source File

SOURCE=.\bot.h
# End Source File
# Begin Source File

SOURCE=.\bot_client.h
# End Source File
# Begin Source File

SOURCE=.\bot_func.h
# End Source File
# Begin Source File

SOURCE=.\bot_globaldefs.h
# End Source File
# Begin Source File

SOURCE=.\bot_modid.h
# End Source File
# Begin Source File

SOURCE=.\bot_names.h
# End Source File
# Begin Source File

SOURCE=.\bot_navigate.h
# End Source File
# Begin Source File

SOURCE=.\bot_weapons.h
# End Source File
# Begin Source File

SOURCE=.\bot_wpdir.h
# End Source File
# Begin Source File

SOURCE=.\bot_wpstat.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\BotNNDefs.h
# End Source File
# Begin Source File

SOURCE=.\cbase.h
# End Source File
# Begin Source File

SOURCE=.\CBotBase.h
# End Source File
# Begin Source File

SOURCE=.\CBotCS.h
# End Source File
# Begin Source File

SOURCE=.\CBotDOD.h
# End Source File
# Begin Source File

SOURCE=.\CCommand.h
# End Source File
# Begin Source File

SOURCE=.\cdll_dll.h
# End Source File
# Begin Source File

SOURCE=.\Chat.h
# End Source File
# Begin Source File

SOURCE=.\ChatBot.h
# End Source File
# Begin Source File

SOURCE=.\ChatHost.h
# End Source File
# Begin Source File

SOURCE=.\ChatHostData.h
# End Source File
# Begin Source File

SOURCE=.\CLTMem.h
# End Source File
# Begin Source File

SOURCE=.\Commandfunc.h
# End Source File
# Begin Source File

SOURCE=..\common\const.h
# End Source File
# Begin Source File

SOURCE=.\CParser.h
# End Source File
# Begin Source File

SOURCE=.\CPersonality.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\CqSOM2d.h
# End Source File
# Begin Source File

SOURCE=.\CRadio.h
# End Source File
# Begin Source File

SOURCE=..\common\crc.h
# End Source File
# Begin Source File

SOURCE=.\CSDecals.h
# End Source File
# Begin Source File

SOURCE=.\CSenses.h
# End Source File
# Begin Source File

SOURCE=.\CSkill.h
# End Source File
# Begin Source File

SOURCE=.\CState.h
# End Source File
# Begin Source File

SOURCE=.\CSTMem.h
# End Source File
# Begin Source File

SOURCE=.\CTask.h
# End Source File
# Begin Source File

SOURCE=..\engine\custom.h
# End Source File
# Begin Source File

SOURCE=..\common\cvardef.h
# End Source File
# Begin Source File

SOURCE=..\engine\edict.h
# End Source File
# Begin Source File

SOURCE=..\engine\eiface.h
# End Source File
# Begin Source File

SOURCE=.\engine.h
# End Source File
# Begin Source File

SOURCE=.\enginecallback.h
# End Source File
# Begin Source File

SOURCE=..\common\entity_state.h
# End Source File
# Begin Source File

SOURCE=..\common\event_flags.h
# End Source File
# Begin Source File

SOURCE=.\extdll.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\fNBorKernel.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\GA.H
# End Source File
# Begin Source File

SOURCE=..\NNSim\Genom.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\GenomToken.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\GlobalCqSOMDefs.h
# End Source File
# Begin Source File

SOURCE=.\globalvars.h
# End Source File
# Begin Source File

SOURCE=.\Gnome.h
# End Source File
# Begin Source File

SOURCE=..\common\in_buttons.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\LinkBProp.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\LinkBPropM.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\Impl\Liste.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\Defs\ListeDef.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\Impl\Listeelem.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\Defs\ListeelemDef.h
# End Source File
# Begin Source File

SOURCE=.\LListElem.h
# End Source File
# Begin Source File

SOURCE=.\monsterevent.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuralNet.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuralNetBProp.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuralNetBPropM.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuralNetDefs.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuronBProp.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\NeuronBPropM.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\NNSimService.h
# End Source File
# Begin Source File

SOURCE=.\NNWeapon.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\nSOMErr.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\nVec.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\nVecErr.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\PatternBProp.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\PatternElemBProp.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_info.h
# End Source File
# Begin Source File

SOURCE=..\engine\progdefs.h
# End Source File
# Begin Source File

SOURCE=.\saverestore.h
# End Source File
# Begin Source File

SOURCE=.\schedule.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\som.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\SOMPattern.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\SOMPatternElem.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\SOM\SOMPatternErr.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# Begin Source File

SOURCE=.\waypoint.h
# End Source File
# Begin Source File

SOURCE=..\common\weaponinfo.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\WeightBProp.h
# End Source File
# Begin Source File

SOURCE=..\NNSim\WeightBPropM.h
# End Source File
# Begin Source File

SOURCE=.\WorldGnome.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\docs\bot.cfg
# End Source File
# Begin Source File

SOURCE=..\docs\bot_names.txt
# End Source File
# Begin Source File

SOURCE=..\docs\bot_skill.txt
# End Source File
# Begin Source File

SOURCE=.\notes.txt
# End Source File
# Begin Source File

SOURCE=..\DOCS\pfnRegUserMsg.txt
# End Source File
# Begin Source File

SOURCE=..\docs\texts.txt
# End Source File
# Begin Source File

SOURCE=..\DOCS\VERSIONS.TXT
# End Source File
# End Target
# End Project
