# Microsoft Developer Studio Project File - Name="SFX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** �ҏW���Ȃ��ł������� **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SFX - Win32 Debug
!MESSAGE ����͗L����Ҳ�̧�قł͂���܂���B ������ۼު�Ă�����ނ��邽�߂ɂ� NMAKE ���g�p���Ă��������B
!MESSAGE [Ҳ�̧�ق̴���߰�] ����ނ��g�p���Ď��s���Ă�������
!MESSAGE 
!MESSAGE NMAKE /f "SFX.mak".
!MESSAGE 
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "SFX.mak" CFG="SFX - Win32 Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "SFX - Win32 Release" ("Win32 (x86) Application" �p)
!MESSAGE "SFX - Win32 Debug" ("Win32 (x86) Application" �p)
!MESSAGE "SFX - Win32 ReleaseD" ("Win32 (x86) Application" �p)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SFX - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /Gz /MT /W3 /GX /O1 /I "..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "EXTRACT_ONLY" /D "NO_REGISTRY" /D "_SFX" /D "_NO_CRYPTO" /D "WIN_LONG_PATH" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /opt:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SFX - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Gz /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "EXTRACT_ONLY" /D "NO_REGISTRY" /D "_SFX" /D "_NO_CRYPTO" /D "WIN_LONG_PATH" /Yu"StdAfx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "SFX - Win32 ReleaseD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseD"
# PROP BASE Intermediate_Dir "ReleaseD"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseD"
# PROP Intermediate_Dir "ReleaseD"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "EXTRACT_ONLY" /D "EXCLUDE_COM" /D "NO_REGISTRY" /D "_SFX" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /Gz /MD /W3 /GX /O1 /I "..\..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "EXTRACT_ONLY" /D "NO_REGISTRY" /D "_SFX" /D "_NO_CRYPTO" /D "WIN_LONG_PATH" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"C:\UTIL\7zWinSR.exe"
# SUBTRACT BASE LINK32 /debug /nodefaultlib
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"ReleaseD/7zSDJC.sfx" /opt:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SFX - Win32 Release"
# Name "SFX - Win32 Debug"
# Name "SFX - Win32 ReleaseD"
# Begin Group "Spec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# End Group
# Begin Group "7z"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zDecode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zDecode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zExtract.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zFolderOutStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zFolderOutStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zIn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zIn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\7z\7zRegister.cpp
# End Source File
# End Group
# Begin Group "Archive Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Archive\Common\CoderMixer2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\CoderMixer2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\CoderMixer2MT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\CoderMixer2MT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\CrossThreadProgress.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\CrossThreadProgress.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\ItemNameUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\ItemNameUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\OutStreamWithCRC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Archive\Common\OutStreamWithCRC.h
# End Source File
# End Group
# Begin Group "Compress"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Compress\Bcj2Coder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\Bcj2Register.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\BcjCoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\BcjRegister.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\BranchCoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\CopyCoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\CopyRegister.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\Lzma2Decoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\Lzma2Decoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\Lzma2Register.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\LzmaDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\LzmaRegister.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\PpmdDecoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Compress\PpmdRegister.cpp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\Common\CommandLineParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\CommandLineParser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\CRC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\CRC.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\CWrappers.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\CWrappers.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\IntToString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\IntToString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\MyString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\MyString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\MyVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\MyVector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\NewHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\NewHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\StringConvert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\StringConvert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\TextConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\TextConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\UTFConvert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\UTFConvert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\Wildcard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Common\Wildcard.h
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Group "Control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\Windows\Control\Dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Control\Dialog.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\Windows\DLL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\DLL.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Error.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Error.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\FileDir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\FileDir.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\FileFind.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\FileFind.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\FileIO.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\FileIO.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\FileName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\FileName.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\PropVariant.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\PropVariant.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\ResourceString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\ResourceString.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Shell.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Shell.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Synchronization.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Synchronization.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Window.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\Windows\Window.h
# End Source File
# End Group
# Begin Group "7z Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Common\CreateCoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\CreateCoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\FileStreams.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\FileStreams.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\FilterCoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\FilterCoder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\InBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\InBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\LimitedStreams.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\LimitedStreams.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\LockedStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\LockedStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\OutBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\OutBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\ProgressUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\ProgressUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StreamBinder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StreamBinder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StreamObjects.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StreamObjects.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StreamUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\StreamUtils.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\VirtThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Common\VirtThread.h
# End Source File
# End Group
# Begin Group "UI"

# PROP Default_Filter ""
# Begin Group "Explorer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\UI\Explorer\MyMessages.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Explorer\MyMessages.h
# End Source File
# End Group
# Begin Group "UI Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\UI\Common\ArchiveOpenCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Common\ArchiveOpenCallback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Common\DefaultName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Common\DefaultName.h
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Common\ExtractMode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Common\LoadCodecs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Common\LoadCodecs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Common\OpenArchive.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\Common\OpenArchive.h
# End Source File
# End Group
# End Group
# Begin Group "File Manager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\UI\FileManager\FormatUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\UI\FileManager\FormatUtils.h
# End Source File
# End Group
# Begin Group "C"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\C\7zCrc.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\7zCrc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\7zCrcOpt.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Alloc.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Alloc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Bra.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Bra86.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\CpuArch.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\CpuArch.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Lzma2Dec.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Lzma2Dec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\LzmaDec.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\LzmaDec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Ppmd7.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Ppmd7.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Ppmd7Dec.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Threads.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\C\Threads.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ExtractCallback.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtractCallback.h
# End Source File
# Begin Source File

SOURCE=.\ExtractDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtractDialog.h
# End Source File
# Begin Source File

SOURCE=.\ExtractEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\setup.ico
# End Source File
# End Target
# End Project