// 7-zip32.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//
#include "stdafx.h"
#include "7-zip32.h"
#include "Dialog.h"
#include "resource.h"
#include "MyOpenArchive.h"
#include "SplitCmdLine.h"
#include "Common/StdOutStream.h"

#include "Common/UTFConvert.h"
#include "Windows/FileDir.h"

extern int g_CodePage;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			g_hInstance = (HINSTANCE)hModule;
			OSVERSIONINFO versionInfo;
			versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
			if (!::GetVersionEx(&versionInfo))
				g_IsNT = false;
			else
				g_IsNT = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);
			break;
		}
	case DLL_PROCESS_DETACH:
		COpenArchive::RemoveAll();
		break;
	}
    return TRUE;
}

int WINAPI SevenZip(const HWND _hwnd, LPCSTR _szCmdLine, LPSTR _szOutput, const DWORD _dwSize)
{
	if (g_StdOut.GetThread())
		return g_StdOut.SetLastError(ERROR_ALREADY_RUNNING);

	CSplitCmdLine scl;
	if (!scl.Split(_szCmdLine))
		return g_StdOut.SetLastError(ERROR_COMMAND_NAME);

	NWindows::NFile::NDirectory::CTempFile sfxFile;
	if (scl.m_bSfx && scl.IsUpdateCommands())
	{
		CSfxDialog dlgSfx(_hwnd);
		if (dlgSfx.DoModal() == IDCANCEL)
			return g_StdOut.SetLastError(ERROR_USER_CANCEL);

		UInt32 sfxSize, processedSize;
		NWindows::NFile::NIO::COutFile file;
		sfxFile.CreateRandomInTempFolder(FTEXT("sfx"), &file);
		g_StdOut.SetSfxPath(sfxFile.GetPath());
		LPVOID lpResource = CSfxDialog::LoadSfxResource(sfxSize);
		file.Write(lpResource, sfxSize, processedSize);
		AString utf8String;
		ConvertUnicodeToUTF8(dlgSfx.GetConfigText(), utf8String);
		file.Write((const char*)utf8String, utf8String.Length(), processedSize);
		file.Close();
	}

	g_StdOut.SetCommandLine(scl);
	UString strCurrentDirectory;
	if (scl.m_lpBaseDirectory)
	{
		::NWindows::NFile::NDirectory::MyGetCurrentDirectory(strCurrentDirectory);
		if (::NWindows::NFile::NDirectory::MySetCurrentDirectory(::MultiByteToUnicodeString(scl.m_lpBaseDirectory, scl.m_codePage)) == false)
		{
			g_StdOut << "Base directory isn't found.";
			g_StdOut.CopyBuf(_szOutput, _dwSize);
			g_StdOut.ReSet();
			return g_StdOut.SetLastError(ERROR_COMMAND_NAME);
		}
	}

	g_StdOut.CreateMainThread();

	CProgressDialog dlgProgress(_hwnd);
	g_StdOut.SetProgressDialog(&dlgProgress);
	dlgProgress.SetShowDialog(!scl.m_bHide);
	dlgProgress.SetProgressMode(scl.m_argv[0][0]);
	dlgProgress.CreateModal();

	DWORD dwThreadId;
	::WaitForSingleObject(g_StdOut.GetThread(), INFINITE);
	::GetExitCodeThread(g_StdOut.GetThread(), &dwThreadId);
	::CloseHandle(g_StdOut.GetThread());
	g_StdOut.CopyBuf(_szOutput, _dwSize);
	g_StdOut.ReSet();
	g_CodePage = -1;
	if (scl.m_lpBaseDirectory)
		::NWindows::NFile::NDirectory::MySetCurrentDirectory(strCurrentDirectory);
    return dwThreadId;
}

WORD WINAPI SevenZipGetVersion()
{
	g_StdOut.SetLastError(0);
	return (SEVENZIP32_VERSION / 10000);
}

// ��{�I�ɈӖ��Ȃ�
static BOOL bCursorMode = FALSE;
static int  nCursorInterval = 80;
static BOOL bBackGroundMode = FALSE;
BOOL WINAPI SevenZipGetCursorMode() { g_StdOut.SetLastError(0); return bCursorMode; }
BOOL WINAPI SevenZipSetCursorMode(const BOOL _CursorMode) { g_StdOut.SetLastError(0); bCursorMode = _CursorMode; return TRUE; }
WORD WINAPI SevenZipGetCursorInterval() { g_StdOut.SetLastError(0); return nCursorInterval; }
BOOL WINAPI SevenZipSetCursorInterval(const WORD _Interval) { g_StdOut.SetLastError(0); nCursorInterval = _Interval; return TRUE; }
BOOL WINAPI SevenZipGetBackGroundMode() { g_StdOut.SetLastError(0); return bBackGroundMode; }
BOOL WINAPI SevenZipSetBackGroundMode(const BOOL _BackGroundMode) { g_StdOut.SetLastError(0); bBackGroundMode = _BackGroundMode; return TRUE; }

BOOL WINAPI SevenZipGetRunning()
{
	g_StdOut.SetLastError(0);
	return (g_StdOut.GetThread() != NULL);
}

// �o�[�W��������\��
BOOL WINAPI SevenZipConfigDialog(const HWND _hwnd, LPSTR _szOptionBuffer, const int _iMode)
{
	if (g_StdOut.GetThread())
		return g_StdOut.SetLastError(ERROR_ALREADY_RUNNING);
	g_StdOut.SetLastError(0);
	CConfigDialog dlg(_hwnd);
	return dlg.DoModal();
}

BOOL WINAPI SevenZipCheckArchive(LPCSTR _szFileName, const int _iMode)
{
	COpenArchive arc;
	return (g_StdOut.SetLastError(arc.OpenCheck(g_StdOut.ConvertUnicodeString(_szFileName), _iMode)) == S_OK);
}

int WINAPI SevenZipGetFileCount(LPCSTR _szArcFile)
{
	HARC hArc = SevenZipOpenArchive(NULL, _szArcFile, 0);
	COpenArchive* pOpenArchive = COpenArchive::FindObject(hArc);
	if (pOpenArchive == NULL)
		return -1;
	int nCount = pOpenArchive->GetFileCount();
	delete pOpenArchive;
	return nCount;
}

BOOL WINAPI SevenZipQueryFunctionList(const int _iFunction)
{
	g_StdOut.SetLastError(0);
	switch (_iFunction)
	{
	case ISARC:
	case ISARC_GET_VERSION:
	case ISARC_GET_CURSOR_INTERVAL:
	case ISARC_SET_CURSOR_INTERVAL:
	case ISARC_GET_BACK_GROUND_MODE:
	case ISARC_SET_BACK_GROUND_MODE:
	case ISARC_GET_CURSOR_MODE:
	case ISARC_SET_CURSOR_MODE:
	case ISARC_GET_RUNNING:
		
	case ISARC_CHECK_ARCHIVE:
	case ISARC_CONFIG_DIALOG:
	case ISARC_GET_FILE_COUNT:
	case ISARC_QUERY_FUNCTION_LIST:
//	case ISARC_HOUT:
//	case ISARC_STRUCTOUT:
//	case ISARC_GET_ARC_FILE_INFO:
		
	case ISARC_OPEN_ARCHIVE:
	case ISARC_CLOSE_ARCHIVE:
	case ISARC_FIND_FIRST:
	case ISARC_FIND_NEXT:
//	case ISARC_EXTRACT:
//	case ISARC_ADD:
//	case ISARC_MOVE:
//	case ISARC_DELETE:
	case ISARC_SETOWNERWINDOW:
	case ISARC_CLEAROWNERWINDOW:
	case ISARC_SETOWNERWINDOWEX:
	case ISARC_KILLOWNERWINDOWEX:
		
	case ISARC_GET_ARC_FILE_NAME:
	case ISARC_GET_ARC_FILE_SIZE:
	case ISARC_GET_ARC_ORIGINAL_SIZE:
	case ISARC_GET_ARC_COMPRESSED_SIZE:
	case ISARC_GET_ARC_RATIO:
	case ISARC_GET_ARC_DATE:
	case ISARC_GET_ARC_TIME:
//	case ISARC_GET_ARC_OS_TYPE:
//	case ISARC_GET_ARC_IS_SFX_FILE:
	case ISARC_GET_ARC_WRITE_TIME_EX:
	case ISARC_GET_ARC_CREATE_TIME_EX:
	case ISARC_GET_ARC_ACCESS_TIME_EX:
//	case ISARC_GET_ARC_CREATE_TIME_EX2:
//	case ISARC_GET_ARC_WRITE_TIME_EX2:
	case ISARC_GET_FILE_NAME:
	case ISARC_GET_ORIGINAL_SIZE:
	case ISARC_GET_COMPRESSED_SIZE:
	case ISARC_GET_RATIO:
	case ISARC_GET_DATE:
	case ISARC_GET_TIME:
	case ISARC_GET_CRC:
	case ISARC_GET_ATTRIBUTE:
//	case ISARC_GET_OS_TYPE:
	case ISARC_GET_METHOD:
	case ISARC_GET_WRITE_TIME:
	case ISARC_GET_CREATE_TIME:
	case ISARC_GET_ACCESS_TIME:
	case ISARC_GET_WRITE_TIME_EX:
	case ISARC_GET_CREATE_TIME_EX:
	case ISARC_GET_ACCESS_TIME_EX:
//	case ISARC_SET_ENUM_MEMBERS_PROC:
//	case ISARC_CLEAR_ENUM_MEMBERS_PROC:
	case ISARC_GET_ARC_FILE_SIZE_EX:
	case ISARC_GET_ARC_ORIGINAL_SIZE_EX:
	case ISARC_GET_ARC_COMPRESSED_SIZE_EX:
	case ISARC_GET_ORIGINAL_SIZE_EX:
	case ISARC_GET_COMPRESSED_SIZE_EX:
	case ISARC_SETOWNERWINDOWEX64:
	case ISARC_KILLOWNERWINDOWEX64:
//	case ISARC_SET_ENUM_MEMBERS_PROC64:
//	case ISARC_CLEAR_ENUM_MEMBERS_PROC64:
//	case ISARC_OPEN_ARCHIVE2:
//	case ISARC_GET_ARC_READ_SIZE:
//	case ISARC_GET_ARC_READ_SIZE_EX:
	case ISARC_SET_PRIORITY:
	case ISARC_GET_LAST_ERROR:
	case ISARC_SET_UNICODE_MODE:
	case ISARC_SET_DEFAULT_PASSWORD:
	case ISARC_GET_DEFAULT_PASSWORD:
	case ISARC_PASSWORD_DIALOG:
	case ISARC_SFX_CONFIG_DIALOG:
	case ISARC_SFX_FILE_STORING:
		return TRUE;
	default:
		break;
	}
	return FALSE;
}

HARC WINAPI SevenZipOpenArchive(const HWND _hwnd, LPCSTR _szFileName, const DWORD _dwMode)
{
	COpenArchive* pOpenArchive = new COpenArchive;
	if (pOpenArchive)
	{
		if (pOpenArchive->Open(g_StdOut.ConvertUnicodeString(_szFileName), _dwMode) == FALSE)
		{
			delete pOpenArchive;
			return NULL;
		}
	}
	return (HARC)pOpenArchive;
}

int WINAPI SevenZipCloseArchive(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	delete pOpenArchive;
	return g_StdOut.SetLastError(0);
}

int WINAPI SevenZipFindFirst(HARC _harc, LPCSTR _szWildName, INDIVIDUALINFO *_lpSubInfo)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
		return g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
	return pOpenArchive->FindFirst(g_StdOut.ConvertUnicodeString(_szWildName), _lpSubInfo);
}

int WINAPI SevenZipFindNext(HARC _harc, INDIVIDUALINFO *_lpSubInfo)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
		return g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
	return pOpenArchive->FindNext(_lpSubInfo);
}

int WINAPI SevenZipGetArcFileName(HARC _harc, LPSTR _lpBuffer, const int _nSize)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
		return g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
	return g_StdOut.SetLastError(g_StdOut.WideCharToMultiByte(pOpenArchive->GetArcFileName(), _lpBuffer, _nSize) == 0 ? 0 : ERROR_INVALID_VALUE);
}

DWORD WINAPI SevenZipGetArcFileSize(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	g_StdOut.SetLastError(0);
	DWORD dwSize = pOpenArchive->GetArcFileSize();
	g_StdOut.SetLastError(dwSize == -1 ? ERROR_TOO_BIG : 0);
	return dwSize;
}

DWORD WINAPI SevenZipGetArcOriginalSize(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	if (!pOpenArchive->IsSearchMode())
	{
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return -1;
	}
	DWORD dwSize = pOpenArchive->GetArcOriginalSize();
	g_StdOut.SetLastError(dwSize == -1 ? ERROR_TOO_BIG : 0);
	return dwSize;
}

DWORD WINAPI SevenZipGetArcCompressedSize(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	if (!pOpenArchive->IsSearchMode())
	{
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return -1;
	}
	if (pOpenArchive->IsSolid())
	{
		g_StdOut.SetLastError(ERROR_GET_INFORMATION);
		return -1;
	}
	DWORD dwSize = pOpenArchive->GetArcCompressedSize();
	g_StdOut.SetLastError(dwSize == -1 ? ERROR_TOO_BIG : 0);
	return dwSize;
}

WORD WINAPI SevenZipGetArcRatio(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	if (!pOpenArchive->IsSearchMode())
	{
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return -1;
	}
	if (pOpenArchive->IsSolid())
	{
		g_StdOut.SetLastError(ERROR_GET_INFORMATION);
		return 0;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcRatio();
}

WORD WINAPI SevenZipGetArcDate(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcDate();
}

WORD WINAPI SevenZipGetArcTime(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcTime();
}

// �G���g���̂݁B�G���[���ȊO�A�K��0��Ԃ�
UINT WINAPI SevenZipGetArcOSType(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcOSType();
}

// �G���g���̂݁B�G���[���ȊO�A�K��0��Ԃ�
int WINAPI SevenZipIsSFXFile(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->IsSFXFile();
}

int WINAPI SevenZipGetFileName(HARC _harc, LPSTR _lpBuffer, const int _nSize)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
		return g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
	LPCWSTR lpFileName = pOpenArchive->GetFileName();
	if (lpFileName == NULL)
		return g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
	return g_StdOut.SetLastError(g_StdOut.WideCharToMultiByte(lpFileName, _lpBuffer, _nSize) == 0 ? 0 : ERROR_INVALID_VALUE);
}

DWORD WINAPI SevenZipGetOriginalSize(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	DWORD dwSize = pOpenArchive->GetOriginalSize();
	g_StdOut.SetLastError(dwSize == -1 ? (pOpenArchive->IsSearchMode() ? ERROR_TOO_BIG : ERROR_NOT_SEARCH_MODE) : 0);
	return dwSize;
}

DWORD WINAPI SevenZipGetCompressedSize(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	if (!pOpenArchive->IsSearchMode())
	{
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return -1;
	}
	if (pOpenArchive->IsSolid())
	{
		g_StdOut.SetLastError(ERROR_GET_INFORMATION);
		return 0;
	}
	DWORD dwSize = pOpenArchive->GetCompressedSize();
	g_StdOut.SetLastError(dwSize == -1 ? ERROR_TOO_BIG : 0);
	return dwSize;
}

WORD WINAPI SevenZipGetRatio(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	if (!pOpenArchive->IsSearchMode())
	{
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return -1;
	}
	if (pOpenArchive->IsSolid())
	{
		g_StdOut.SetLastError(ERROR_GET_INFORMATION);
		return 0;
	}
	WORD wRatio = pOpenArchive->GetRatio();
	g_StdOut.SetLastError(0);
	return wRatio;
}

WORD WINAPI SevenZipGetDate(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	WORD wDate = pOpenArchive->GetDate();
	g_StdOut.SetLastError(wDate == -1 ? (pOpenArchive->IsSearchMode() ? ERROR_GET_TIME : ERROR_NOT_SEARCH_MODE) : 0);
	return wDate;
}

WORD WINAPI SevenZipGetTime(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	WORD wTime = pOpenArchive->GetTime();
	g_StdOut.SetLastError(wTime == -1 ? (pOpenArchive->IsSearchMode() ? ERROR_GET_TIME : ERROR_NOT_SEARCH_MODE) : 0);
	return wTime;
}

DWORD WINAPI SevenZipGetCRC(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	DWORD dwCRC = pOpenArchive->GetCRC();
	g_StdOut.SetLastError(pOpenArchive->IsSearchMode() ? 0 : ERROR_NOT_SEARCH_MODE);
	return dwCRC;
}

int WINAPI SevenZipGetAttribute(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	return pOpenArchive->GetAttribute();
	int nAttr = pOpenArchive->GetAttribute();
	g_StdOut.SetLastError(nAttr == -1 ? ERROR_NOT_SEARCH_MODE : 0);
	return nAttr;
}

// �G���g���̂݁B�G���[���ȊO�A�K��0��Ԃ�
UINT WINAPI SevenZipGetOSType(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	UINT nOS = pOpenArchive->GetOSType();
	g_StdOut.SetLastError(nOS == -1 ? ERROR_NOT_SEARCH_MODE : 0);
	return nOS;
}

int WINAPI SevenZipGetMethod(HARC _harc, LPSTR _lpBuffer, const int _nSize)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
		return g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
	LPCWSTR lpMethod = pOpenArchive->GetMethod();
	if (lpMethod == NULL)
		return g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
	if (_nSize == 8)
		g_StdOut.GetCompactMethod(lpMethod, pOpenArchive->GetArchiveType(), _lpBuffer);
	else if(g_StdOut.WideCharToMultiByte(lpMethod, _lpBuffer, _nSize))
		return g_StdOut.SetLastError(ERROR_INVALID_VALUE);
	return g_StdOut.SetLastError(0);
}

DWORD WINAPI SevenZipGetWriteTime(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	DWORD dwTime = pOpenArchive->GetWriteTime();
	g_StdOut.SetLastError(dwTime == -1 ? (pOpenArchive->IsSearchMode() ? ERROR_GET_TIME : ERROR_NOT_SEARCH_MODE) : 0);
	return dwTime;
}

DWORD WINAPI SevenZipGetCreateTime(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	DWORD dwTime = pOpenArchive->GetCreateTime();
	switch (dwTime)
	{
	case -1:
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return -1;
	case 0:
		dwTime = pOpenArchive->GetWriteTime();
		g_StdOut.SetLastError(ERROR_GET_TIME);
		break;
	default:
		g_StdOut.SetLastError(0);
	}
	return dwTime;
}

DWORD WINAPI SevenZipGetAccessTime(HARC _harc)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return -1;
	}
	DWORD dwTime = pOpenArchive->GetAccessTime();
	switch (dwTime)
	{
	case -1:
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return -1;
	case 0:
		dwTime = pOpenArchive->GetWriteTime();
		g_StdOut.SetLastError(ERROR_GET_TIME);
		break;
	default:
		g_StdOut.SetLastError(0);
	}
	return dwTime;
}

BOOL WINAPI SevenZipGetWriteTimeEx(HARC _harc, FILETIME *_lpftLastWriteTime)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	BOOL bRes = pOpenArchive->GetWriteTimeEx(_lpftLastWriteTime);
	g_StdOut.SetLastError(bRes ? 0 : (pOpenArchive->IsSearchMode() ? ERROR_GET_TIME : ERROR_NOT_SEARCH_MODE));
	return bRes;
}

BOOL WINAPI SevenZipGetCreateTimeEx(HARC _harc, FILETIME *_lpftCreationTime)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	BOOL bRes = pOpenArchive->GetCreateTimeEx(_lpftCreationTime);
	if (bRes)
		g_StdOut.SetLastError(0);
	else if (!pOpenArchive->IsSearchMode())
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
	else
	{
		bRes = pOpenArchive->GetWriteTimeEx(_lpftCreationTime);
		g_StdOut.SetLastError(ERROR_GET_TIME);
	}
	return bRes;
}

BOOL WINAPI SevenZipGetAccessTimeEx(HARC _harc, FILETIME *_lpftLastAccessTime)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	BOOL bRes = pOpenArchive->GetAccessTimeEx(_lpftLastAccessTime);
	if (bRes)
		g_StdOut.SetLastError(0);
	else if (!pOpenArchive->IsSearchMode())
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
	else
	{
		bRes = pOpenArchive->GetWriteTimeEx(_lpftLastAccessTime);
		g_StdOut.SetLastError(ERROR_GET_TIME);
	}
	return bRes;
}

BOOL WINAPI SevenZipGetArcCreateTimeEx(HARC _harc, FILETIME *_lpftCreationTime)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcCreateTimeEx(_lpftCreationTime);
}

BOOL WINAPI SevenZipGetArcAccessTimeEx(HARC _harc, FILETIME *_lpftLastAccessTime)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcAccessTimeEx(_lpftLastAccessTime);
}

BOOL WINAPI SevenZipGetArcWriteTimeEx(HARC _harc, FILETIME *_lpftLastWriteTime)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcWriteTimeEx(_lpftLastWriteTime);
}

BOOL WINAPI SevenZipGetArcFileSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcFileSizeEx(_lpllSize);
}

BOOL WINAPI SevenZipGetArcOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcOriginalSizeEx(_lpllSize);
}

BOOL WINAPI SevenZipGetArcCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetArcCompressedSizeEx(_lpllSize);
}

BOOL WINAPI SevenZipGetOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	if (!pOpenArchive->IsSearchMode())
	{
		*_lpllSize = 0; 
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return FALSE;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetOriginalSizeEx(_lpllSize);
}

BOOL WINAPI SevenZipGetCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize)
{
	COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
	if (pOpenArchive == NULL)
	{
		g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
		return FALSE;
	}
	if (!pOpenArchive->IsSearchMode())
	{
		*_lpllSize = 0; 
		g_StdOut.SetLastError(ERROR_NOT_SEARCH_MODE);
		return FALSE;
	}
	g_StdOut.SetLastError(0);
	return pOpenArchive->GetCompressedSizeEx(_lpllSize);
}

BOOL WINAPI SevenZipSetOwnerWindow(HWND _hwnd)
{
	g_StdOut.SetLastError(g_StdOut.GetThread() ? ERROR_ALREADY_RUNNING : 0);
	return CProgressDialog::SetOwnerWindow(_hwnd);
}

BOOL WINAPI SevenZipClearOwnerWindow()
{
	g_StdOut.SetLastError(g_StdOut.GetThread() ? ERROR_ALREADY_RUNNING : 0);
	return CProgressDialog::ClearOwnerWindow();
}

BOOL WINAPI SevenZipSetOwnerWindowEx(HWND _hwnd, LPARCHIVERPROC _lpArcProc)
{
	g_StdOut.SetLastError(g_StdOut.GetThread() ? ERROR_ALREADY_RUNNING : 0);
	return CProgressDialog::SetOwnerWindowEx(_hwnd, _lpArcProc);
}

BOOL WINAPI SevenZipKillOwnerWindowEx(HWND _hwnd)
{
	g_StdOut.SetLastError(g_StdOut.GetThread() ? ERROR_ALREADY_RUNNING : 0);
	return CProgressDialog::KillOwnerWindowEx(_hwnd);
}

BOOL WINAPI SevenZipSetOwnerWindowEx64(HWND _hwnd, LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize)
{
	g_StdOut.SetLastError(g_StdOut.GetThread() ? ERROR_ALREADY_RUNNING : 0);
	return CProgressDialog::SetOwnerWindowEx64(_hwnd, _lpArcProc, _dwStructSize);
}

BOOL WINAPI SevenZipKillOwnerWindowEx64(HWND _hwnd)
{
	g_StdOut.SetLastError(g_StdOut.GetThread() ? ERROR_ALREADY_RUNNING : 0);
	return CProgressDialog::KillOwnerWindowEx64(_hwnd);
}

WORD WINAPI SevenZipGetSubVersion()
{
	g_StdOut.SetLastError(0);
	return (SEVENZIP32_VERSION % 10000);
}

int WINAPI SevenZipGetArchiveType(LPCSTR _szFileName)
{
	COpenArchive arc;
	HRESULT res = arc.OpenCheck(g_StdOut.ConvertUnicodeString(_szFileName), CHECKARCHIVE_RAPID);
	g_StdOut.SetLastError(res);
	if (FAILED(res))
		return -1;
	return arc.GetArchiveType();
}

BOOL WINAPI SevenZipSetPriority(const int _nPriority)
{
	return g_StdOut.SetLastError(g_StdOut.SetPriority(_nPriority)) == S_OK;
}

int WINAPI SevenZipGetLastError(LPDWORD _lpdwSystemError)
{
	return g_StdOut.GetLastError(_lpdwSystemError);
}

int WINAPI SevenZipSetDefaultPassword(HARC _harc, LPCSTR _szPassword)
{
	int nRes = 0;
	if (_harc == NULL)
		g_StdOut.SetDefaultPassword(_szPassword);
	else
	{
		COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
		if (pOpenArchive == NULL)
			nRes = ERROR_HARC_ISNOT_OPENED;
		else
			pOpenArchive->SetDefaultPassword(_szPassword);
	}
	return g_StdOut.SetLastError(nRes);
}

DWORD WINAPI SevenZipGetDefaultPassword(HARC _harc, LPSTR _szPassword, DWORD _dwSize)
{
	g_StdOut.SetLastError(0);
	LPCWSTR lpPassword;
	if (_harc == NULL)
		lpPassword = g_StdOut.GetDefaultPassword();
	else
	{
		COpenArchive* pOpenArchive = COpenArchive::FindObject(_harc);
		if (pOpenArchive == NULL)
		{
			g_StdOut.SetLastError(ERROR_HARC_ISNOT_OPENED);
			return -1;
		}
		lpPassword = pOpenArchive->GetDefaultPassword();
	}
	if (lpPassword == NULL)
	{
		g_StdOut.SetLastError(ERROR_PASSWORD_FILE);
		return -1;
	}
	AString strPassword;
	if (g_StdOut.GetUnicodeMode())
		::ConvertUnicodeToUTF8(lpPassword, strPassword);
	else
		strPassword = ::GetOemString(lpPassword);
	if ((DWORD)strPassword.Length() >= _dwSize)
	{
		g_StdOut.SetLastError(ERROR_INVALID_VALUE);
		return strPassword.Length() + 1;
	}
	::lstrcpyn(_szPassword, strPassword, _dwSize);
	return 0;
}
// �����܂Ń`�F�b�N
int WINAPI SevenZipPasswordDialog(HWND _hwnd, LPSTR _szBuffer, DWORD _dwSize)
{
	CPasswordDialog dlg(_hwnd, IDS_ENTER_PASSWORD);
	if (dlg.DoModal() == IDCANCEL)
	{
		g_StdOut.SetLastError(ERROR_USER_CANCEL);
		return -1;
	}
	AString strPassword;
	if (g_StdOut.GetUnicodeMode())
		::ConvertUnicodeToUTF8(dlg.GetPassword(), strPassword);
	else
		strPassword = ::GetOemString(dlg.GetPassword());
	::lstrcpynA(_szBuffer, strPassword, _dwSize);
	return g_StdOut.SetLastError((DWORD)strPassword.Length() < _dwSize ? 0 : ERROR_INVALID_VALUE);
}

BOOL WINAPI SevenZipSetUnicodeMode(BOOL _bUnicode)
{
	return g_StdOut.SetUnicodeMode(_bUnicode);
}

int WINAPI SevenZipSfxConfigDialog(HWND _hwnd, LPSTR _szBuffer, DWORD _dwSize)
{
	CSfxDialog dlgSfx(_hwnd);
	if (dlgSfx.DoModal() == IDCANCEL)
	{
		g_StdOut.SetLastError(ERROR_USER_CANCEL);
		return -1;
	}

	AString utf8String;
	ConvertUnicodeToUTF8(dlgSfx.GetConfigText(), utf8String);
	::lstrcpynA(_szBuffer, utf8String, _dwSize);
	return g_StdOut.SetLastError((DWORD)utf8String.Length() < _dwSize ? 0 : ERROR_INVALID_VALUE);
}

BOOL WINAPI SevenZipSfxFileStoring(LPCSTR _szFileName)
{
	UInt32 sfxSize, processedSize;
	LPVOID lpResource = CSfxDialog::LoadSfxResource(sfxSize);
	if (!lpResource)
	{
		g_StdOut.SetLastError(ERROR_FATAL);
		return FALSE;
	}

	NWindows::NFile::NIO::COutFile file;
	if (file.Create(us2fs(g_StdOut.ConvertUnicodeString(_szFileName)), false) == false)
	{
		g_StdOut.SetLastError(ERROR_FILE_CREATE);
		return FALSE;
	}
	if (file.Write(lpResource, sfxSize, processedSize) == false)
	{
		g_StdOut.SetLastError(ERROR_CANNOT_WRITE);
		return FALSE;
	}
	file.Close();
	g_StdOut.SetLastError(0);
	return TRUE;
}