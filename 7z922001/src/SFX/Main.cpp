// Main.cpp

#include "StdAfx.h"

#include "Common/MyInitGuid.h"

#include "Common/CommandLineParser.h"
#include "Common/StringConvert.h"
#include "Common/TextConfig.h"

#include "Windows/DLL.h"
#include "Windows/FileDir.h"
#include "Windows/FileFind.h"
#include "Windows/FileIO.h"
#include "Windows/FileName.h"
#include "Windows/NtCheck.h"
#include "Windows/ResourceString.h"

#include "../../../UI/Explorer/MyMessages.h"

#include "../../SFXSetup/ExtractEngine.h"		// �p�X�ύX

#include "resource.h"

#include "ExtractDialog.h"				// �ǉ�
#include "Windows/ResourceString.h"		// �ǉ�

using namespace NWindows;

HINSTANCE g_hInstance;

static CFSTR kTempDirPrefix = FTEXT("7zSDJC");		// �ύX

#define _SHELL_EXECUTE

static bool ReadDataString(CFSTR fileName, LPCSTR startID,
    LPCSTR endID, AString &stringResult)
{
  stringResult.Empty();
  NFile::NIO::CInFile inFile;
  if (!inFile.Open(fileName))
    return false;
  const int kBufferSize = (1 << 12);

  Byte buffer[kBufferSize];
  int signatureStartSize = MyStringLen(startID);
  int signatureEndSize = MyStringLen(endID);
  
  UInt32 numBytesPrev = 0;
  bool writeMode = false;
  UInt64 posTotal = 0;
  for (;;)
  {
    if (posTotal > (1 << 20))
      return (stringResult.IsEmpty());
    UInt32 numReadBytes = kBufferSize - numBytesPrev;
    UInt32 processedSize;
    if (!inFile.Read(buffer + numBytesPrev, numReadBytes, processedSize))
      return false;
    if (processedSize == 0)
      return true;
    UInt32 numBytesInBuffer = numBytesPrev + processedSize;
    UInt32 pos = 0;
    for (;;)
    {
      if (writeMode)
      {
        if (pos > numBytesInBuffer - signatureEndSize)
          break;
        if (memcmp(buffer + pos, endID, signatureEndSize) == 0)
          return true;
        char b = buffer[pos];
        if (b == 0)
          return false;
        stringResult += b;
        pos++;
      }
      else
      {
        if (pos > numBytesInBuffer - signatureStartSize)
          break;
        if (memcmp(buffer + pos, startID, signatureStartSize) == 0)
        {
          writeMode = true;
          pos += signatureStartSize;
        }
        else
          pos++;
      }
    }
    numBytesPrev = numBytesInBuffer - pos;
    posTotal += pos;
    memmove(buffer, buffer + pos, numBytesPrev);
  }
}

static char kStartID[] = ",!@Install@!UTF-8!";
static char kEndID[] = ",!@InstallEnd@!";

class CInstallIDInit
{
public:
  CInstallIDInit()
  {
    kStartID[0] = ';';
    kEndID[0] = ';';
  };
} g_CInstallIDInit;


#ifndef UNDER_CE
class CCurrentDirRestorer
{
  FString m_CurrentDirectory;
public:
  CCurrentDirRestorer() { NFile::NDirectory::MyGetCurrentDirectory(m_CurrentDirectory); }
  ~CCurrentDirRestorer() { RestoreDirectory();}
  bool RestoreDirectory() const { return NFile::NDirectory::MySetCurrentDirectory(m_CurrentDirectory); }
};
#endif

#define NT_CHECK_FAIL_ACTION ShowErrorMessage(L"Unsupported Windows version"); return 1;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /* hPrevInstance */,
    #ifdef UNDER_CE
    LPWSTR
    #else
    LPSTR
    #endif
    /* lpCmdLine */,int /* nCmdShow */)
{
  g_hInstance = (HINSTANCE)hInstance;

  NT_CHECK

  // InitCommonControls();

  UString archiveName, switches;
  #ifdef _SHELL_EXECUTE
  UString executeFile, executeParameters;
  #endif
  NCommandLineParser::SplitCommandLine(GetCommandLineW(), archiveName, switches);

  FString fullPath;
  NDLL::MyGetModuleFileName(fullPath);

  switches.Trim();
  bool assumeYes = false;
  if (switches.Left(2).CompareNoCase(UString(L"-y")) == 0)
  {
    assumeYes = true;
    switches = switches.Mid(2);
    switches.Trim();
  }

  AString config;
  if (!ReadDataString(fullPath, kStartID, kEndID, config))
  {
    if (!assumeYes)
      ShowErrorMessageRes(IDS_CANT_LOAD_CONFIG_INFO);		// �ύX
    return 1;
  }

  UString dirPrefix = L"." WSTRING_PATH_SEPARATOR;
  UString appLaunched;
  bool showProgress = true;
  UString friendlyName;		// �ǉ�
  UString installPrompt;	// �ǉ�
  bool isInstaller;			// �ǉ�
  if (!config.IsEmpty())
  {
    CObjectVector<CTextConfigPair> pairs;
    if (!GetTextConfig(config, pairs))
    {
      if (!assumeYes)
        ShowErrorMessageRes(IDS_CONFIG_FAILED);		// �ύX
      return 1;
    }
    friendlyName = GetTextConfigValue(pairs, L"Title");			// �ύX
    installPrompt = GetTextConfigValue(pairs, L"BeginPrompt");	// �ύX
    isInstaller = GetTextConfigValue(pairs, L"IsInstaller").CompareNoCase(L"yes") == 0;	// �ǉ�
    if (isInstaller)	// �ǉ�
    {					// �ǉ�
    UString progress = GetTextConfigValue(pairs, L"Progress");
    if (progress.CompareNoCase(L"no") == 0)
      showProgress = false;
    int index = FindTextConfigItem(pairs, L"Directory");
    if (index >= 0)
      dirPrefix = pairs[index].String;
    if (!installPrompt.IsEmpty() && !assumeYes)
    {
      if (MessageBoxW(0, installPrompt, friendlyName, MB_YESNO |
          MB_ICONQUESTION) != IDYES)
        return 0;
    }
    appLaunched = GetTextConfigValue(pairs, L"RunProgram");
    
    #ifdef _SHELL_EXECUTE
    executeFile = GetTextConfigValue(pairs, L"ExecuteFile");
    executeParameters = GetTextConfigValue(pairs, L"ExecuteParameters") + switches;
    #endif
	}	// �ǉ�
  }

  NFile::NDirectory::CTempDir tempDir;

  /* �ǉ� */
	UString tempDirPath;
  	if (isInstaller)
	{
		if (!tempDir.Create(kTempDirPrefix))
		{
			if (!assumeYes)
				ShowErrorMessageRes(IDS_CANT_CREATE_TEMP_FOLDER);
			return 1;
		}
		tempDirPath = GetUnicodeString(tempDir.GetPath());
	}
	else
	{
		tempDirPath = fullPath.Mid(0, fullPath.ReverseFind('\\') + 1);
		if (!assumeYes)
		{
			if (friendlyName.IsEmpty())
				friendlyName = NWindows::MyLoadStringW(IDS_EXTRACT_FRIENDLY_NAME);
			if (installPrompt.IsEmpty())
				installPrompt = NWindows::MyLoadStringW(IDS_EXTRACT_INSTALL_PROMPT);
			CExtractDialog dlg;
			if (dlg.Create(friendlyName, installPrompt, tempDirPath, 0) != IDOK)
				return 0;
			tempDirPath = dlg.GetFolderName();
		}
	}
  /* �ǉ������܂�*/
  /* �폜 
  if (!tempDir.Create(kTempDirPrefix))
  {
    if (!assumeYes)
      ShowErrorMessage(L"Can not create temp folder archive");
    return 1;
  }
     �폜�����܂� */
  CCodecs *codecs = new CCodecs;
  CMyComPtr<IUnknown> compressCodecsInfo = codecs;
  HRESULT result = codecs->Load();
  if (result != S_OK)
  {
    ShowErrorMessageRes(IDS_CANT_LOAD_CODECS);			// �ύX
    return 1;
  }

//  FString tempDirPath = tempDir.GetPath();�@	// �폜
  {
    bool isCorrupt = false;
    UString errorMessage;
    HRESULT result = ExtractArchive(codecs, fullPath, tempDirPath, showProgress,
      isCorrupt, errorMessage);
    
    if (result != S_OK)
    {
      if (!assumeYes)
      {
        if (result == S_FALSE || isCorrupt)
        {
          errorMessage = NWindows::MyLoadStringW(IDS_EXTRACTION_ERROR_MESSAGE);
          result = E_FAIL;
        }
        if (result != E_ABORT && !errorMessage.IsEmpty())
          ::MessageBoxW(0, errorMessage, NWindows::MyLoadStringW(IDS_EXTRACTION_ERROR_TITLE), MB_ICONERROR);
      }
      return 1;
    }
  }
	if (!isInstaller)	// �ǉ�
		return 0;		// �ǉ�

  #ifndef UNDER_CE
  CCurrentDirRestorer currentDirRestorer;
  if (!NFile::NDirectory::MySetCurrentDirectory(tempDir.GetPath()))
    return 1;
  #endif
  
  HANDLE hProcess = 0;
#ifdef _SHELL_EXECUTE
  if (!executeFile.IsEmpty())
  {
    CSysString filePath = GetSystemString(executeFile);
    SHELLEXECUTEINFO execInfo;
    execInfo.cbSize = sizeof(execInfo);
    execInfo.fMask = SEE_MASK_NOCLOSEPROCESS
      #ifndef UNDER_CE
      | SEE_MASK_FLAG_DDEWAIT
      #endif
      ;
    execInfo.hwnd = NULL;
    execInfo.lpVerb = NULL;
    execInfo.lpFile = filePath;

    if (!switches.IsEmpty())
      executeParameters += switches;

    CSysString parametersSys = GetSystemString(executeParameters);
    if (parametersSys.IsEmpty())
      execInfo.lpParameters = NULL;
    else
      execInfo.lpParameters = parametersSys;

    execInfo.lpDirectory = NULL;
    execInfo.nShow = SW_SHOWNORMAL;
    execInfo.hProcess = 0;
    /* BOOL success = */ ::ShellExecuteEx(&execInfo);
    UINT32 result = (UINT32)(UINT_PTR)execInfo.hInstApp;
    if(result <= 32)
    {
      if (!assumeYes)
        ShowErrorMessageRes(IDS_CANT_OPEN_FILE);			// �ύX
      return 1;
    }
    hProcess = execInfo.hProcess;
  }
  else
#endif
  {
    if (appLaunched.IsEmpty())
    {
      appLaunched = L"setup.exe";
      if (!NFile::NFind::DoesFileExist(us2fs(appLaunched)))
      {
        if (!assumeYes)
          ShowErrorMessageRes(IDS_CANT_FIND_SETUP);			// �ύX
        return 1;
      }
    }
    
    {
      FString s2 = tempDirPath;
      NFile::NName::NormalizeDirPathPrefix(s2);
      appLaunched.Replace(L"%%T" WSTRING_PATH_SEPARATOR, fs2us(s2));
    }
    
    appLaunched.Replace(L"%%T", fs2us(tempDirPath));

    if (!switches.IsEmpty())
    {
      appLaunched += L' ';
      appLaunched += switches;
    }
    STARTUPINFO startupInfo;
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.lpReserved = 0;
    startupInfo.lpDesktop = 0;
    startupInfo.lpTitle = 0;
    startupInfo.dwFlags = 0;
    startupInfo.cbReserved2 = 0;
    startupInfo.lpReserved2 = 0;
    
    PROCESS_INFORMATION processInformation;
    
    CSysString appLaunchedSys = GetSystemString(dirPrefix + appLaunched);
    
    BOOL createResult = CreateProcess(NULL, (LPTSTR)(LPCTSTR)appLaunchedSys,
      NULL, NULL, FALSE, 0, NULL, NULL /*tempDir.GetPath() */,
      &startupInfo, &processInformation);
    if (createResult == 0)
    {
      if (!assumeYes)
        ShowLastErrorMessage();
      return 1;
    }
    ::CloseHandle(processInformation.hThread);
    hProcess = processInformation.hProcess;
  }
  if (hProcess != 0)
  {
    WaitForSingleObject(hProcess, INFINITE);
    ::CloseHandle(hProcess);
  }
  return 0;
}
