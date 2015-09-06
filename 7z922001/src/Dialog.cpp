// Dialog.cpp: ダイアログ関係
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dialog.h"
#include "resource.h"
#include "Common/StdOutStream.h"

#include "Windows/FileIO.h"
#include "Windows/FileDir.h"
#include "Common/UTFConvert.h"

#include <commctrl.h>
#include <crtdbg.h>

extern BOOL HandlerRoutine();

// 簡易SHGetFileInfo(W版Win9x系対応)
static const FARPROC kSHGetFileInfo = ::GetProcAddress(::GetModuleHandle("SHELL32.DLL"), "SHGetFileInfoW");
DWORD MySHGetFileInfo(LPCWSTR pszPath, DWORD dwFileAttributes, SHFILEINFOW *psfi, UINT uFlags)
{
	if (g_IsNT)
	{
		DWORD (WINAPI* MySHGetFileInfoW)(LPCWSTR, DWORD, SHFILEINFOW*, UINT, UINT);
		(FARPROC&)MySHGetFileInfoW = kSHGetFileInfo;
		return MySHGetFileInfoW(pszPath, dwFileAttributes, psfi, sizeof(SHFILEINFOW), uFlags);
	}
	SHFILEINFOA shfi;
	DWORD dwRes = ::SHGetFileInfoA(::GetOemString(pszPath), dwFileAttributes, &shfi, sizeof(SHFILEINFOA), uFlags);
	psfi->hIcon = shfi.hIcon;
	psfi->iIcon = shfi.iIcon;
	psfi->dwAttributes = shfi.dwAttributes;
	if (uFlags & (SHGFI_DISPLAYNAME | SHGFI_ICONLOCATION))
		::MultiByteToWideChar(CP_OEMCP, 0, shfi.szDisplayName, -1, psfi->szDisplayName, MAX_PATH);
	else
		psfi->szDisplayName[0] = '\0';
	if (uFlags & SHGFI_TYPENAME)
		::MultiByteToWideChar(CP_OEMCP, 0, shfi.szTypeName, -1, psfi->szTypeName, 80);
	else
		psfi->szTypeName[0] = '\0';
	return dwRes;
}

// 現在のプロセスで最後にアクティイブだったウィンドウを取得
BOOL CALLBACK GetProcessLastActivePopup(HWND hWnd, LPARAM lParam)
{
	DWORD dwProcessId;
	::GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (::GetCurrentProcessId() == dwProcessId &&
		::GetWindow(hWnd, GW_OWNER) == NULL)
	{
		HWND* phWnd= (HWND*)lParam;
		*phWnd = ::GetLastActivePopup(hWnd);
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CDialog : ダイアログ基本クラス
//////////////////////////////////////////////////////////////////////

CDialog::CDialog()
{
	m_hWnd = NULL;
	m_hWndParent = NULL;
	m_nTemplateID = 0;
	m_bLoopExit = FALSE;
}

CDialog::~CDialog()
{

}

// リストビューに文字列設定（W版Win9x系対応)
void CDialog::ListView_SetItemTextW(HWND hwnd, int i, int iSubItem, LPCWSTR pszText)
{
	if (g_IsNT)
	{
		LVITEMW lvitem = { LVIF_TEXT, i, iSubItem, 0, 0, (LPWSTR)pszText };
		::SendMessageW(hwnd, LVM_SETITEMTEXTW, i, (LPARAM)&lvitem);
	}
	else
	{
		AString strOem = ::GetOemString(pszText);
		ListView_SetItemText(hwnd, i, iSubItem, strOem.GetBuffer(0));
	}
}

BOOL CDialog::DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_INITDIALOG)
	{
		((CDialog*)lParam)->m_hWnd = hDlg;
		::SetWindowLong(hDlg, GWL_USERDATA, lParam);
	}
	CDialog* pDlg = (CDialog*)::GetWindowLong(hDlg, GWL_USERDATA);
	return pDlg ? pDlg->DialogProc(uMsg, wParam, lParam) : 0;
}

BOOL CDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

// メッセージループ
void CDialog::Loop()
{
	MSG msg;
	while (!m_bLoopExit && ::GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.hwnd == NULL || !::IsDialogMessage(m_hWnd, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if (msg.hwnd == m_hWndParent && msg.message == 0)
				::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}
	}
	m_bLoopExit = FALSE;
}

// ウィンドウを親ウィンドウの中心に移動する
void CDialog::MoveCenter()
{
	RECT rcOwner;
	RECT rcChild;
	RECT rcDesktop;

	::GetWindowRect(m_hWndParent ? m_hWndParent : ::GetDesktopWindow(), &rcOwner);
	::GetWindowRect(m_hWnd, &rcChild);
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);
	int x = rcChild.left + (rcOwner.left + rcOwner.right - rcChild.left - rcChild.right) / 2;
	int y = rcChild.top + (rcOwner.top + rcOwner.bottom - rcChild.top - rcChild.bottom) / 2;
	if (x < rcDesktop.left) x = rcDesktop.left;
	if (y < rcDesktop.top) y = rcDesktop.top;
	if (x + rcChild.right - rcChild.left > rcDesktop.right) x = rcDesktop.right - (rcChild.right - rcChild.left);
	if (y + rcChild.bottom - rcChild.top > rcDesktop.bottom) y = rcDesktop.bottom - (rcChild.bottom - rcChild.top);
	::SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

// モードレスダイアログでモーダルダイアログの働きをさせるダイアログを作成。
BOOL CDialog::CreateModal()
{
	if (g_IsNT)
	{
		if (!::CreateDialogParamW(g_hInstance, MAKEINTRESOURCEW(m_nTemplateID), m_hWndParent, CDialog::DialogProc, (LPARAM)this))
			return FALSE;
	}
	else
	{
		if (!::CreateDialogParamA(g_hInstance, MAKEINTRESOURCEA(m_nTemplateID), m_hWndParent, CDialog::DialogProc, (LPARAM)this))
			return FALSE;
	}
	Loop();
	return TRUE;
}

// ダイアログのコントロールの文字列を設定（W版Win9x系対応)
BOOL CDialog::SetDlgItemTextW(int nID, LPCWSTR lpszString)
{
	if (g_IsNT)
		return ::SetDlgItemTextW(m_hWnd, nID, lpszString);
	return ::SetDlgItemTextA(m_hWnd, nID, GetOemString(lpszString));
}

// ダイアログのコントロールの文字列を取得（W版Win9x系対応)
int CDialog::GetDlgItemTextW(int nID, UString& rString)
{
	rString.Empty();
	if (g_IsNT)
	{
		int nLen = ::GetWindowTextLengthW(::GetDlgItem(m_hWnd, nID));
		if (nLen)
		{
			++nLen;
			::GetDlgItemTextW(m_hWnd, nID, rString.GetBuffer(nLen), nLen);
			rString.ReleaseBuffer();
		}
	}
	else
	{
		AString string;
		int nLen = ::GetWindowTextLengthA(::GetDlgItem(m_hWnd, nID));
		if (nLen)
		{
			++nLen;
			::GetDlgItemTextA(m_hWnd, nID, string.GetBuffer(nLen), nLen);
			string.ReleaseBuffer();
		}
		rString = GetUnicodeString(string);
	}
	return rString.Length();
}

// ダイアログのコントロールに入るようパスを短縮した文字列に変換
BOOL CDialog::PathGetDlgItemPathW(int nID, UString& rString)
{
	HWND hItemWnd = ::GetDlgItem(m_hWnd, nID);
	HDC hDC = ::GetDC(hItemWnd);
	HGDIOBJ hFont = (HGDIOBJ)::SendMessage(hItemWnd, WM_GETFONT, 0, 0);
	HGDIOBJ hOldFont = ::SelectObject(hDC, hFont);
	RECT rc;
	int nRes;
	::GetClientRect(hItemWnd, &rc);
	if (g_IsNT)
	{
		nRes = ::DrawTextExW(hDC, rString.GetBuffer(rString.Length() + 4), rString.Length(), &rc, DT_PATH_ELLIPSIS | DT_MODIFYSTRING | DT_CALCRECT, NULL);
		rString.ReleaseBuffer();
	}
	else
	{
		AString string = GetOemString(rString);
		nRes = ::DrawTextExA(hDC, string.GetBuffer(string.Length() + 4), string.Length(), &rc, DT_PATH_ELLIPSIS | DT_MODIFYSTRING | DT_CALCRECT, NULL);
		string.ReleaseBuffer();
		rString = GetUnicodeString(string);
	}
	::SelectObject(hDC, hOldFont);
	::ReleaseDC(hItemWnd, hDC);
	return (nRes != 0);
}

// モーダルダイアログを作成
int CDialog::DoModal()
{
	if (g_IsNT)
		return ::DialogBoxParamW(g_hInstance, MAKEINTRESOURCEW(m_nTemplateID), m_hWndParent, CDialog::DialogProc, (LPARAM)this);
	return ::DialogBoxParamA(g_hInstance, MAKEINTRESOURCEA(m_nTemplateID), m_hWndParent, CDialog::DialogProc, (LPARAM)this);
}

//////////////////////////////////////////////////////////////////////
// CProgressDialog : 経過表示ダイアログ
//////////////////////////////////////////////////////////////////////

static const UINT wm_arcextract = ::RegisterWindowMessageA(WM_ARCEXTRACT);
static HWND s_hOwnerWnd = NULL;
static HWND s_hOwnerWndEx = NULL;
static HWND s_hOwnerWndEx64 = NULL;
static LPARCHIVERPROC s_lpArcProc = NULL;
static LPARCHIVERPROC s_lpArcProc64 = NULL;
static DWORD s_dwStructSize = 0;
static union EXTRACTINGINFOS
{
	EXTRACTINGINFO ei;
	EXTRACTINGINFOEX eix;
	EXTRACTINGINFOEX32 eix32;
	EXTRACTINGINFOEX64 eix64;
} s_eis;

#define PROGRESS_TIMER	100

// メッセージ送信先ウィンドウを設定
BOOL CProgressDialog::SetOwnerWindow(HWND hWnd)
{
	if (s_hOwnerWnd || g_StdOut.GetThread())
		return FALSE;
	s_hOwnerWnd = hWnd;
	s_dwStructSize = sizeof(EXTRACTINGINFO);
	return TRUE;
}

// SevenZipSetOwnerWindowEx用設定保存
BOOL CProgressDialog::SetOwnerWindowEx(HWND hWnd, LPARCHIVERPROC lpArcProc)
{
	if (s_hOwnerWndEx || g_StdOut.GetThread() || (hWnd ==NULL && lpArcProc == NULL))
		return FALSE;
	s_hOwnerWndEx = hWnd;
	s_lpArcProc = lpArcProc;
	s_dwStructSize = sizeof(EXTRACTINGINFOEX);
	return TRUE;
}

// SevenZipSetOwnerWindowEx64用設定保存
BOOL CProgressDialog::SetOwnerWindowEx64(HWND hWnd, LPARCHIVERPROC lpArcProc, DWORD dwStructSize)
{
	if (s_hOwnerWndEx64 || g_StdOut.GetThread() || (hWnd ==NULL && lpArcProc == NULL))
		return FALSE;
	s_hOwnerWndEx64 = hWnd;
	s_lpArcProc64 = lpArcProc;
	s_dwStructSize = dwStructSize;
	return TRUE;
}

// オーナーウィンドウを解除
BOOL CProgressDialog::ClearOwnerWindow()
{
	if (s_hOwnerWnd == NULL || g_StdOut.GetThread())
		return FALSE;
	s_hOwnerWnd = NULL;
	s_dwStructSize = 0;
	return TRUE;
}

// SevenZipSetOwnerWindowEx用設定解除
BOOL CProgressDialog::KillOwnerWindowEx(HWND hWnd)
{
	if ((hWnd && hWnd != s_hOwnerWndEx) || g_StdOut.GetThread())
		return FALSE;
	s_hOwnerWndEx = NULL;
	s_lpArcProc = NULL;
	s_dwStructSize = 0;
	return TRUE;
}

// SevenZipSetOwnerWindowEx64用設定解除
BOOL CProgressDialog::KillOwnerWindowEx64(HWND hWnd)
{
	if ((hWnd && hWnd != s_hOwnerWndEx64) || g_StdOut.GetThread())
		return FALSE;
	s_hOwnerWndEx64 = NULL;
	s_lpArcProc64 = NULL;
	s_dwStructSize = 0;
	return TRUE;
}

CProgressDialog::CProgressDialog(HWND hParentWnd)
{
	m_nTemplateID = IDD_PROGRESS;
	m_hWndParent = hParentWnd;
	m_nTotalSize = 0;
	m_nCompletedSize = 0;
	m_nNextItemPos = 0;
	m_nLastFilePos = 0;
	m_nItemIndex = 0;
	m_dwStartTime = 0;
	m_hDetailWnd = NULL;
	m_bShow = FALSE;
	m_cMode = '\0';
	m_nWndHigh[0] = 0;
	m_nWndHigh[1] = 0;
	m_nArchiveType = 0;
	m_bSolid = false;
}

CProgressDialog::~CProgressDialog()
{
	for (int i = 0; i < m_wfis.Size(); ++i)
	{
		LPWORKFILEINFO lpWfi = &m_wfis[i];
		free((void*)lpWfi->lpSrcFileName);
		free((void*)lpWfi->lpDestFilePath);
		free((void*)lpWfi->lpMethod);
	}
}

BOOL CProgressDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return OnInitDialog();
	case WM_CLOSE:
		if (lParam)
		{
			if (m_bShow)
				::EnableWindow(m_hWndParent, TRUE);
			::DestroyWindow(m_hWnd);
			m_bLoopExit = TRUE;
			break;
		}
		HandlerRoutine();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			HandlerRoutine();
			break;
		case IDC_DETAIL:
			ShowDetail(::IsDlgButtonChecked(m_hWnd, IDC_DETAIL));
			break;
		case IDC_BACK_GROUND:
			{
				int nPriority = g_StdOut.GetPriority() == THREAD_PRIORITY_IDLE ? THREAD_PRIORITY_NORMAL : g_StdOut.GetPriority();
				if (::IsDlgButtonChecked(m_hWnd, IDC_BACK_GROUND) == BST_CHECKED)
					nPriority = THREAD_PRIORITY_IDLE;
				::SetThreadPriority(g_StdOut.GetThread(), nPriority);
				break;
			}
		default:
			return FALSE;
		}
		break;
	case WM_TIMER:
		if (wParam == PROGRESS_TIMER)
			SetProgressTime();
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CProgressDialog::OnInitDialog()
{
	if (m_bShow)
	{
		char lpBuffer[64];
		m_detailItem.Add(IDS_COUNT_DOWN);
		m_detailItem.Add(IDS_ELAPSED_TIME);
		m_detailItem.Add(IDS_PROCESSING_SPEED);
		m_detailItem.Add(IDS_PROCESS_FILE_SIZE);
		switch (m_cMode)
		{
		case 'a':
		case 'A':
			m_cMode = 'a';
			::LoadString(g_hInstance, IDS_PROGRESS_COMPRESS, lpBuffer, 64);
			m_detailItem.Add(IDS_FILE_TYPE);
			m_detailItem.Add(IDS_WRITE_TIME);
			m_detailItem.Add(IDS_ATTRIBUTE);
			break;
		case 'e':
		case 'E':
		case 'x':
		case 'X':
			m_cMode = 'x';
			::LoadString(g_hInstance, IDS_PROGRESS_EXTRACT, lpBuffer, 64);
			m_detailItem.Add(IDS_FILE_TYPE);
			m_detailItem.Add(IDS_WRITE_TIME);
			m_detailItem.Add(IDS_ATTRIBUTE);
			m_detailItem.Add(IDS_RATIO);
			m_detailItem.Add(IDS_METHOD);
			m_detailItem.Add(IDS_CRC);
			break;
		case 'd':
		case 'D':
			m_cMode = 'd';
			::LoadString(g_hInstance, IDS_PROGRESS_DELETE, lpBuffer, 64);
			break;
		case 'u':
		case 'U':
			m_cMode = 'a';
			::LoadString(g_hInstance, IDS_PROGRESS_UPDATE, lpBuffer, 64);
			m_detailItem.Add(IDS_FILE_TYPE);
			m_detailItem.Add(IDS_WRITE_TIME);
			m_detailItem.Add(IDS_ATTRIBUTE);
			break;
		default:
			::ResumeThread(g_StdOut.GetThread());
			return FALSE;
		}
		::SetWindowText(m_hWnd, lpBuffer);
		::SendDlgItemMessage(m_hWnd, IDP_PROGRESS, PBM_SETRANGE32, 0, 1000);
		::CheckDlgButton(m_hWnd, IDC_BACK_GROUND, g_StdOut.GetPriority() == THREAD_PRIORITY_IDLE ? BST_CHECKED : BST_UNCHECKED);

		m_hDetailWnd = ::GetDlgItem(m_hWnd, IDL_DETAIL);
		LVCOLUMN lvcol = { LVCF_FMT | LVCF_WIDTH | LVCF_TEXT, LVCFMT_LEFT, 0, lpBuffer, 64 };
		::LoadString(g_hInstance, IDS_PROPERTY, lpBuffer, 64);
		ListView_InsertColumn(m_hDetailWnd, 0, &lvcol);
		::LoadString(g_hInstance, IDS_VALUE, lpBuffer, 64);
		ListView_InsertColumn(m_hDetailWnd, 1, &lvcol);

		LVITEM lvitem;
		lvitem.mask = TVIF_TEXT;
		lvitem.pszText = lpBuffer;
		lvitem.cchTextMax = 64;
		lvitem.iSubItem = 0;
		for (lvitem.iItem = 0; lvitem.iItem < m_detailItem.Size(); ++lvitem.iItem)
		{
			::LoadString(g_hInstance, m_detailItem[lvitem.iItem], lpBuffer, 64);
			ListView_InsertItem(m_hDetailWnd, &lvitem);
		}

		RECT rc;
		::GetWindowRect(m_hWnd, &rc);
		m_nWndHigh[1] = rc.bottom - rc.top;							// 詳細表示時のウィンドウの高さ
		::GetWindowRect(m_hDetailWnd, &rc);
		POINT pt = { rc.left, rc.top };
		::ScreenToClient(m_hWnd, &pt);
		m_nWndHigh[0]= pt.y + ::GetSystemMetrics(SM_CYCAPTION);		// 詳細非表示時のウィンドウの高さ
		ShowDetail(FALSE);

		::GetClientRect(m_hDetailWnd, &rc);
		ListView_SetColumnWidth(m_hDetailWnd, 0, LVSCW_AUTOSIZE);
		ListView_SetColumnWidth(m_hDetailWnd, 1, rc.right - ListView_GetColumnWidth(m_hDetailWnd, 0));

		MoveCenter();
		::ShowWindow(m_hWnd, SW_SHOW);
		::EnableWindow(m_hWndParent, FALSE);

		m_dwStartTime = ::GetTickCount();
	}
	::SetThreadPriority(g_StdOut.GetThread(), g_StdOut.GetPriority());
	::ResumeThread(g_StdOut.GetThread());
	return FALSE;
}

// 処理書庫ファイル名を設定
void CProgressDialog::SetArchiveFile(LPCWSTR lpFileName)
{
	if (m_bShow)
	{
		UString strPath = lpFileName;
		PathGetDlgItemPathW(IDS_ARCHIVE_FILE, strPath);
		SetDlgItemTextW(IDS_ARCHIVE_FILE, strPath);
	}
	else if (s_dwStructSize)
	{	
		// ARCEXTRACT_OPEN 送信
		ZeroMemory(&s_eis, s_dwStructSize);
		switch (s_dwStructSize)
		{
		case sizeof(EXTRACTINGINFO):
			g_StdOut.WideCharToMultiByte(lpFileName, s_eis.ei.szSourceFileName, FNAME_MAX32);
			break;
		case sizeof(EXTRACTINGINFOEX):
			g_StdOut.WideCharToMultiByte(lpFileName, s_eis.eix.exinfo.szSourceFileName, FNAME_MAX32);
			break;
		case sizeof(EXTRACTINGINFOEX32):
			s_eis.eix32.dwStructSize = sizeof(EXTRACTINGINFOEX32);
			g_StdOut.WideCharToMultiByte(lpFileName, s_eis.eix32.exinfo.szSourceFileName, FNAME_MAX32);
			::lstrcpynA(s_eis.eix32.szSourceFileName, s_eis.eix32.exinfo.szSourceFileName, FNAME_MAX32);
			break;
		case sizeof(EXTRACTINGINFOEX64):
			s_eis.eix64.dwStructSize = sizeof(EXTRACTINGINFOEX64);
			g_StdOut.WideCharToMultiByte(lpFileName, s_eis.eix64.exinfo.szSourceFileName, FNAME_MAX32);
			::lstrcpynA(s_eis.eix64.szSourceFileName, s_eis.eix64.exinfo.szSourceFileName, FNAME_MAX32);
			break;
		}
		SendExtractingInfo(ARCEXTRACT_OPEN);
	}
}

// 全体の処理サイズを設定
void CProgressDialog::SetTotal(UINT64 nSize)
{
	m_nTotalSize = nSize;
	if (m_bShow)
	{
		char lpProcessSize[64];
		sprintf(lpProcessSize, "0/%I64u", nSize);
		::SetDlgItemText(m_hWnd, IDS_PROCESS_SIZE, lpProcessSize);
	}
}

// 処理ファイル情報表示
void CProgressDialog::SetWorkFile(LPWORKFILEINFO lpWfi)
{
	m_nNextItemPos += lpWfi->nSrcFileSize;
	if (m_bShow)
	{
		UString strPath = lpWfi->lpSrcFileName;
		PathGetDlgItemPathW(IDS_WORK_FILE, strPath);
		SetDlgItemTextW(IDS_WORK_FILE, strPath);
		SetDetail(lpWfi);
	}
	else if (s_dwStructSize)
	{
		// ARCEXTRACT_BEGIN 送信
		EXTRACTINGINFO exinfo;
		if (m_nTotalSize < 0xFFFFFFFF)
			exinfo.dwFileSize = (DWORD)m_nTotalSize;
		else
			exinfo.dwFileSize = -1;
		g_StdOut.WideCharToMultiByte(lpWfi->lpSrcFileName, exinfo.szSourceFileName, FNAME_MAX32);
		g_StdOut.WideCharToMultiByte(lpWfi->lpDestFilePath, exinfo.szDestFileName, FNAME_MAX32);
		switch (s_dwStructSize)
		{
		case sizeof(EXTRACTINGINFO):
			exinfo.dwWriteSize = s_eis.ei.dwWriteSize;
			s_eis.ei = exinfo;
			break;
		case sizeof(EXTRACTINGINFOEX):
			{
				FILETIME ftLocal;
				exinfo.dwWriteSize = s_eis.eix.exinfo.dwWriteSize;
				s_eis.eix.exinfo = exinfo;
				s_eis.eix.dwCompressedSize = lpWfi->nCompFileSize < 0xFFFFFFFF ? (DWORD)lpWfi->nCompFileSize : -1;
				s_eis.eix.dwCRC = lpWfi->dwCRC;
				s_eis.eix.wRatio = lpWfi->nSrcFileSize ? (WORD)(lpWfi->nCompFileSize * 1000 / lpWfi->nSrcFileSize) : 0;
				// ローカル時間かUTCかチェックする事
				::FileTimeToLocalFileTime(&lpWfi->ftLastWriteTime, &ftLocal);
				::FileTimeToDosDateTime(&ftLocal, &s_eis.eix.wDate, &s_eis.eix.wTime);
				CStdOutStream::GetAttributesString(lpWfi->dwAttributes, m_cMode == 'x', s_eis.eix.szAttribute);
				CStdOutStream::GetCompactMethod(lpWfi->lpMethod, m_nArchiveType, s_eis.eix.szMode);
				break;
			}
		case sizeof(EXTRACTINGINFOEX32):
			exinfo.dwWriteSize = s_eis.eix32.exinfo.dwWriteSize;
			s_eis.eix32.exinfo = exinfo;
			s_eis.eix32.dwFileSize = s_eis.eix32.exinfo.dwFileSize;
			s_eis.eix32.dwAttributes = lpWfi->dwAttributes;
			s_eis.eix32.ftWriteTime = lpWfi->ftLastWriteTime;
			s_eis.eix32.dwCompressedSize = lpWfi->nCompFileSize < 0xFFFFFFFF ? (DWORD)lpWfi->nCompFileSize : -1;
			s_eis.eix32.dwCRC = lpWfi->dwCRC;
			s_eis.eix32.wRatio = lpWfi->nSrcFileSize ? (WORD)(lpWfi->nCompFileSize * 1000 / lpWfi->nSrcFileSize) : 0;
			::lstrcpynA(s_eis.eix32.szSourceFileName, s_eis.eix32.exinfo.szSourceFileName, FNAME_MAX32);
			::lstrcpynA(s_eis.eix32.szDestFileName, s_eis.eix32.exinfo.szDestFileName, FNAME_MAX32);
			CStdOutStream::GetCompactMethod(lpWfi->lpMethod, m_nArchiveType, s_eis.eix32.szMode);
			break;
		case sizeof(EXTRACTINGINFOEX64):
			exinfo.dwWriteSize = s_eis.eix64.exinfo.dwWriteSize;
			s_eis.eix64.exinfo = exinfo;
			s_eis.eix64.llFileSize = m_nTotalSize;
			s_eis.eix64.dwAttributes = lpWfi->dwAttributes;
			s_eis.eix64.ftWriteTime = lpWfi->ftLastWriteTime;
			s_eis.eix64.llCompressedSize = lpWfi->nCompFileSize;
			s_eis.eix64.dwCRC = lpWfi->dwCRC;
			s_eis.eix64.wRatio = lpWfi->nSrcFileSize ? (WORD)(lpWfi->nCompFileSize * 1000 / lpWfi->nSrcFileSize) : 0;
			::lstrcpynA(s_eis.eix64.szSourceFileName, s_eis.eix64.exinfo.szSourceFileName, FNAME_MAX32);
			::lstrcpynA(s_eis.eix64.szDestFileName, s_eis.eix64.exinfo.szDestFileName, FNAME_MAX32);
			CStdOutStream::GetCompactMethod(lpWfi->lpMethod, m_nArchiveType, s_eis.eix64.szMode);
			break;
		}
		SendExtractingInfo(ARCEXTRACT_BEGIN);
	}
}

// ファイル処理進行状況
void CProgressDialog::SetCompleted(UINT64 nSize)
{
	if (m_nCompletedSize == nSize)
		return;
	m_nCompletedSize = nSize;
	if (m_wfis.Size() == 0)
		return;
	while (m_nNextItemPos <= nSize && m_wfis.Size() > m_nItemIndex + 1)
		SetWorkFile(&m_wfis[++m_nItemIndex]);
	if (m_bShow)
	{
		char lpProcessSize[64];
		sprintf(lpProcessSize, "%I64u/%I64u", nSize, m_nTotalSize);
		::SetDlgItemText(m_hWnd, IDS_PROCESS_SIZE, lpProcessSize);
		UINT nRatio = m_nTotalSize ? (UINT)(nSize * 1000 / m_nTotalSize) : 0;
		::SendDlgItemMessage(m_hWnd, IDP_PROGRESS, PBM_SETPOS, nRatio, 0);
		::SetDlgItemText(m_hWnd, IDS_PROCESS_SIZE, lpProcessSize);
		if (m_bShowDetail)
		{
			UINT64 nSrcFileSize = m_wfis[m_nItemIndex].nSrcFileSize;
			UINT64 nPos = nSize - m_nNextItemPos + nSrcFileSize;
			if (nPos > nSrcFileSize)
				nPos = nSrcFileSize;
			sprintf(lpProcessSize, "%I64u/%I64u", nPos, nSrcFileSize);
			ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_PROCESS_FILE_SIZE), 1, lpProcessSize);
		}
	}
	else if (s_dwStructSize)
	{
		// ARCEXTRACT_INPROCESS 送信
		switch (s_dwStructSize)
		{
		case sizeof(EXTRACTINGINFO):
			if (s_eis.ei.dwFileSize == -1)
				s_eis.ei.dwWriteSize = m_nTotalSize ? (DWORD)(nSize * 1000 / m_nTotalSize) : 0;
			else
				s_eis.ei.dwWriteSize = (DWORD)nSize;
			break;
		case sizeof(EXTRACTINGINFOEX):
			if (s_eis.eix.exinfo.dwFileSize == -1)
				s_eis.eix.exinfo.dwWriteSize = m_nTotalSize ? (DWORD)(nSize * 1000 / m_nTotalSize) : 0;
			else
				s_eis.eix.exinfo.dwWriteSize = (DWORD)nSize;
			break;
		case sizeof(EXTRACTINGINFOEX32):
			if (s_eis.eix32.exinfo.dwFileSize == -1)
				s_eis.eix32.exinfo.dwWriteSize = m_nTotalSize ? (DWORD)(nSize * 1000 / m_nTotalSize) : 0;
			else
				s_eis.eix32.exinfo.dwWriteSize = (DWORD)nSize;
			s_eis.eix32.dwWriteSize = s_eis.eix32.exinfo.dwWriteSize;
			break;
		case sizeof(EXTRACTINGINFOEX64):
			if (s_eis.eix64.exinfo.dwFileSize == -1)
				s_eis.eix64.exinfo.dwWriteSize = m_nTotalSize ? (DWORD)(nSize * 1000 / m_nTotalSize) : 0;
			else
				s_eis.eix64.exinfo.dwWriteSize = (DWORD)nSize;
			s_eis.eix64.llWriteSize = nSize;
			break;
		}
		SendExtractingInfo(ARCEXTRACT_INPROCESS);
	}
}

// 詳細設定表示
void CProgressDialog::SetDetail(LPWORKFILEINFO lpWfi)
{
	if (!m_bShowDetail)
		return;
	char lpBuffer[64];
	switch (m_cMode)
	{
	case 'a':
		CStdOutStream::GetAttributesString(lpWfi->dwAttributes, false, lpBuffer);
		ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_ATTRIBUTE), 1, lpBuffer);
		break;
	case 'x':
		CStdOutStream::GetAttributesString(lpWfi->dwAttributes, true, lpBuffer);
		ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_ATTRIBUTE), 1, lpBuffer);
		sprintf(lpBuffer, "%d.%d%%", lpWfi->wRatio / 10, lpWfi->wRatio % 10);
		ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_RATIO), 1, lpBuffer);
		ListView_SetItemTextW(m_hDetailWnd, FindDetailIndex(IDS_METHOD), 1, lpWfi->lpMethod);
		sprintf(lpBuffer, "%.8X", lpWfi->dwCRC);
		ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_CRC), 1, lpBuffer);
		break;
	default:
		return;
	}
	SHFILEINFOW shfi;
	MySHGetFileInfo(lpWfi->lpSrcFileName, 0, &shfi, SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);
	ListView_SetItemTextW(m_hDetailWnd, FindDetailIndex(IDS_FILE_TYPE), 1, shfi.szTypeName);

	FILETIME ftLocal;
	SYSTEMTIME stWrite = { 0 };
	::FileTimeToLocalFileTime(&lpWfi->ftLastWriteTime, &ftLocal);
	::FileTimeToSystemTime(&ftLocal, &stWrite);
	::wsprintf(lpBuffer, "%.4hu/%.2hu/%.2hu %.2hu:%.2hu:%.2hu", stWrite.wYear, stWrite.wMonth, stWrite.wDay, stWrite.wHour, stWrite.wMinute, stWrite.wSecond);
	ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_WRITE_TIME), 1, lpBuffer);
	
	UINT64 nPos = m_nCompletedSize - m_nNextItemPos + lpWfi->nSrcFileSize;
	if (nPos > lpWfi->nSrcFileSize)
		nPos = lpWfi->nSrcFileSize;
	sprintf(lpBuffer, "%I64u/%I64u", nPos, lpWfi->nSrcFileSize);
	ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_PROCESS_FILE_SIZE), 1, lpBuffer);
}

// ファイル情報保存
void CProgressDialog::AddWorkFile(LPCWSTR lpSrcFileName, LPCWSTR lpDestFilePath, DWORD dwAttributes, bool bEncrypted, FILETIME ftLastWriteTime, UINT64 nSrcFileSize, UINT64 nCompFileSize, DWORD dwCRC, LPCWSTR lpMethod)
{
	UString strDestFilePath;
	::NWindows::NFile::NDirectory::MyGetFullPathName(lpDestFilePath, strDestFilePath);
	WORKFILEINFO wfi;
	wfi.lpSrcFileName = _wcsdup(lpSrcFileName);
	wfi.lpDestFilePath = _wcsdup(strDestFilePath);
	wfi.dwAttributes = dwAttributes & (FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY) | (bEncrypted ? FA_ENCRYPTED : 0);
	wfi.ftLastWriteTime = ftLastWriteTime;
	wfi.nSrcFileSize = nSrcFileSize;
	wfi.nCompFileSize = m_bSolid ? nCompFileSize : 0;
	wfi.dwCRC = dwCRC;
	wfi.lpMethod = _wcsdup(lpMethod);
	wfi.wRatio = nSrcFileSize ? (WORD)(nCompFileSize * 1000 / nSrcFileSize) : 0;
	m_wfis.Add(wfi);
	if (m_wfis.Size() == 1)
		SetWorkFile(&wfi);
}

// 詳細表示切り替え
void CProgressDialog::ShowDetail(BOOL bShow)
{
	m_bShowDetail = bShow;
	if (bShow)
	{
		if (m_wfis.Size())
			SetDetail(&m_wfis[m_nItemIndex]);
		SetProgressTime();
		::SetTimer(m_hWnd, PROGRESS_TIMER, 1000, NULL);
	}
	else
		::KillTimer(m_hWnd, PROGRESS_TIMER);
	RECT rc;
	::GetWindowRect(m_hWnd, &rc);
	::MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, m_nWndHigh[bShow ? 1 : 0], TRUE);
}

// オーナーウィンドウに処理を渡す
void CProgressDialog::SendExtractingInfo(int nMode)
{
	if (m_bShow)
		return;

	BOOL bRes = FALSE;
	LPVOID pEis = nMode == ARCEXTRACT_END ? NULL : &s_eis;
	if (s_hOwnerWndEx64 || s_lpArcProc64)
	{
		if (s_lpArcProc64)
			bRes = !s_lpArcProc64(s_hOwnerWndEx64, wm_arcextract, nMode, pEis);
		else
			bRes = ::SendMessage(s_hOwnerWndEx64, wm_arcextract, nMode, (LPARAM)pEis);
	}
	else if (s_hOwnerWndEx || s_lpArcProc)
	{
		if (s_lpArcProc)
			bRes = !s_lpArcProc(s_hOwnerWndEx, wm_arcextract, nMode, pEis);
		else
			bRes = ::SendMessage(s_hOwnerWndEx, wm_arcextract, nMode, (LPARAM)pEis);
	}
	else if (s_hOwnerWnd)
		bRes = ::SendMessage(s_hOwnerWnd, wm_arcextract, nMode, (LPARAM)pEis);
	if (bRes)
		HandlerRoutine();
}

void CProgressDialog::SetProgressTime()
{
	DWORD dwElapse = ::GetTickCount() - m_dwStartTime;
	DWORD dwTime;
	char lpText[32];
	if (m_nCompletedSize)
	{
		dwTime = (DWORD)((m_nTotalSize - m_nCompletedSize) * dwElapse / m_nCompletedSize / 1000);
		sprintf(lpText, "%d:%.2d:%.2d", dwTime / 3600, (dwTime / 60) % 60, dwTime % 60);
		ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_COUNT_DOWN), 1, lpText);
	}
	dwTime = dwElapse / 1000;
	sprintf(lpText, "%d:%.2d:%.2d", dwTime / 3600, (dwTime / 60) % 60, dwTime % 60);
	ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_ELAPSED_TIME), 1, lpText);

	UINT64 nSpeed = m_nCompletedSize * 1000 / dwElapse;
	if (nSpeed < 1000)
		sprintf(lpText, "%d bytes/s", nSpeed);
	else
	{
		static const char* lpUnit[4] = { "KB", "MB", "GB", "TB" };
		*lpText = '\0';
		for (int i = 0; i < 4; ++i)
		{
			UINT64 nConv = (nSpeed * 100) >> 10;
			if (nConv < 100)
				sprintf(lpText, "0.%d %s/s", (int)nConv, lpUnit[i]);
			else if (nConv < 1000)
				sprintf(lpText, "%d.%.2d %s/s", (int)nConv / 100, (int)nConv % 100, lpUnit[i]);
			else if (nConv < 10000)
				sprintf(lpText, "%d.%d %s/s", (int)nConv / 100, ((int)nConv % 100) / 10, lpUnit[i]);
			else if (nConv < 100000)
				sprintf(lpText, "%d %s/s", (int)nConv / 100, lpUnit[i]);
			else
			{
				nSpeed >>= 10;
				continue;
			}
			break;
		}
	}
	ListView_SetItemText(m_hDetailWnd, FindDetailIndex(IDS_PROCESSING_SPEED), 1, lpText);
}

//////////////////////////////////////////////////////////////////////
// CPasswordDialog : パスワード入力ダイアログ
//////////////////////////////////////////////////////////////////////

CPasswordDialog::CPasswordDialog(HWND hParent, UINT uTitleId)
{
	if (hParent)
		m_hWndParent = hParent;
	else
		::EnumWindows(GetProcessLastActivePopup, (LPARAM)&m_hWndParent);
	m_nTemplateID = IDD_PASSWORD;
	m_uTitleId = uTitleId;
}

BOOL CPasswordDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT (WINAPI *MySendMessage)(HWND, UINT, WPARAM, LPARAM) = g_IsNT ? ::SendMessageW : ::SendMessageA;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			char lpBuffer[64];
			::LoadString(g_hInstance, m_uTitleId, lpBuffer, 64);
			::SetWindowText(m_hWnd, lpBuffer);
			m_hPassWnd = ::GetDlgItem(m_hWnd, IDE_PASSWORD);
			m_cMask = (WCHAR)MySendMessage(m_hPassWnd, EM_GETPASSWORDCHAR, 0, 0);
			MoveCenter();
			break;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_MASK:
			MySendMessage(m_hPassWnd, EM_SETPASSWORDCHAR, ::SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED ? 0 : m_cMask, 0);
			::InvalidateRect(::GetDlgItem(m_hWnd, IDE_PASSWORD), NULL, TRUE);
			break;
		case IDOK:
			GetDlgItemTextW(IDE_PASSWORD, m_strPassword);
			::EndDialog(m_hWnd, IDOK);
			break;
		case IDCANCEL:
			::EndDialog(m_hWnd, IDCANCEL);
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CConfirmationDialog : 上書き確認ダイアログ
//////////////////////////////////////////////////////////////////////

CConfirmationDialog::CConfirmationDialog()
{
	m_hWndParent = g_StdOut.GetProgressDialog()->GetBecomeParentWindow();
	m_nTemplateID = IDD_CONFIRMATION;
}

BOOL CConfirmationDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			::SendMessage(::GetDlgItem(m_hWnd, IDS_REPLACE), STM_SETICON, (WPARAM)::LoadIcon(::GetModuleHandle("SHELL32.DLL"), MAKEINTRESOURCE(145)), 0);
			
			FILETIME ftLocal;
			SYSTEMTIME stWrite;
			TCHAR lpFormat[128];
			UString strFileInfo;
			SHFILEINFOW shFileInfoW;
			
			::FileTimeToLocalFileTime(&m_ftNewWrite, &ftLocal);
			::FileTimeToSystemTime(&ftLocal, &stWrite);
			::GetDlgItemText(m_hWnd, IDS_NEW_FILE_INFO, lpFormat, 128);
			UString strPath = m_strNewName;
			PathGetDlgItemPathW(IDS_NEW_FILE_INFO, strPath);
			swprintf(strFileInfo.GetBuffer(m_strNewName.Length() + 128), (LPCWSTR)GetUnicodeString(lpFormat), (LPCWSTR)strPath, stWrite.wYear, stWrite.wMonth, stWrite.wDay, stWrite.wHour, stWrite.wMinute, stWrite.wSecond, m_nNewSize);
			strFileInfo.ReleaseBuffer();
			MySHGetFileInfo(m_strNewName, FILE_ATTRIBUTE_NORMAL, &shFileInfoW, SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_LARGEICON);
			::SendMessage(::GetDlgItem(m_hWnd, IDS_NEW_ICON), STM_SETICON, (WPARAM)shFileInfoW.hIcon, 0);
			SetDlgItemTextW(IDS_NEW_FILE_INFO, strFileInfo);
			
			::FileTimeToLocalFileTime(&m_ftOldWrite, &ftLocal);
			::FileTimeToSystemTime(&ftLocal, &stWrite);
			::GetDlgItemText(m_hWnd, IDS_OLD_FILE_INFO, lpFormat, 128);
			strPath = m_strOldName;
			PathGetDlgItemPathW(IDS_OLD_FILE_INFO, strPath);
			swprintf(strFileInfo.GetBuffer(m_strOldName.Length() + 128), (LPCWSTR)GetUnicodeString(lpFormat), (LPCWSTR)strPath, stWrite.wYear, stWrite.wMonth, stWrite.wDay, stWrite.wHour, stWrite.wMinute, stWrite.wSecond, m_nOldSize);
			strFileInfo.ReleaseBuffer();
			MySHGetFileInfo(m_strOldName, FILE_ATTRIBUTE_NORMAL, &shFileInfoW, SHGFI_ICON | SHGFI_USEFILEATTRIBUTES | SHGFI_LARGEICON);
			::SendMessage(::GetDlgItem(m_hWnd, IDS_OLD_ICON), STM_SETICON, (WPARAM)shFileInfoW.hIcon, 0);
			SetDlgItemTextW(IDS_OLD_FILE_INFO, strFileInfo);

			MoveCenter();
			return TRUE;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDB_YES:
		case IDB_NO:
		case IDB_YES_ALL:
		case IDB_NO_ALL:
		case IDB_AUTO_RENAME:
		case IDCANCEL:
			::EndDialog(m_hWnd, (int)LOWORD(wParam));
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

// ファイル情報の設定
void CConfirmationDialog::SetFileInfo(LPCWSTR lpNewName, UINT64 nNewSize, const FILETIME &ftNewWrite, LPCWSTR lpOldName, UINT64 nOldSize, const FILETIME &ftOldWrite)
{
	::NWindows::NFile::NDirectory::MyGetFullPathName(lpOldName, m_strOldName);
	m_strNewName = lpNewName;
	m_nNewSize = nNewSize;
	m_nOldSize = nOldSize;
	m_ftNewWrite = ftNewWrite;
	m_ftOldWrite = ftOldWrite;
}

//////////////////////////////////////////////////////////////////////
// CConfigDialog : 設定ダイアログ
//////////////////////////////////////////////////////////////////////

CConfigDialog::CConfigDialog(HWND hParentWnd)
{
	m_hWndParent = hParentWnd;
	m_nTemplateID = IDD_CONFIG;
}

BOOL CConfigDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			TCHAR lpVersion[64];
			::wsprintf(lpVersion, "7-zip32.dll version %d.%.2d.%.2d.%.2d", ::SevenZipGetVersion() / 100, ::SevenZipGetVersion() % 100, ::SevenZipGetSubVersion() / 100, ::SevenZipGetSubVersion() % 100);
			::SetDlgItemText(m_hWnd, IDS_VERSION, lpVersion);
			MoveCenter();
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			::EndDialog(m_hWnd, LOWORD(wParam));
			break;
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CSfxDialog : SFX作成ダイアログ
//////////////////////////////////////////////////////////////////////

LPVOID CSfxDialog::LoadSfxResource(UInt32& size)
{
	size = 0;
	HRSRC hResInfo = ::FindResource(g_hInstance, MAKEINTRESOURCE(IDR_7ZSDJC), "SFX");
	if (!hResInfo)
		return NULL;
	HGLOBAL hResData = ::LoadResource(g_hInstance, hResInfo);
	if (!hResData)
		return NULL;
	size = ::SizeofResource(g_hInstance, hResInfo);
	if (!size)
		return NULL;
	return ::LockResource(hResData);
}

CSfxDialog::CSfxDialog(HWND hParentWnd)
{
	m_hWndParent = hParentWnd;
	m_nTemplateID = IDD_SFX;
}

BOOL CSfxDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		::EnableWindow(::GetDlgItem(m_hWnd, IDE_RUN_PROGRAM), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDE_DIRECTORY), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDE_EXECUTE_FILE), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDE_EXECUTE_PARAMETERS), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROGRESS), FALSE);
		::CheckDlgButton(m_hWnd, IDC_PROGRESS, BST_CHECKED);
		MoveCenter();
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			{
				UString strText;
				m_strConfig += L";!@Install@!UTF-8!\r\n";
				GetDlgItemTextW(IDE_TITLE, strText);
				if (strText.Length())
					m_strConfig += L"Title=\"" + strText + L"\"\r\n";
				GetDlgItemTextW(IDE_BEGIN_PROMPT, strText);
				if (strText.Length())
					m_strConfig += L"BeginPrompt=\"" + strText + L"\"\r\n";
				if (::IsDlgButtonChecked(m_hWnd, IDC_IS_INSTALLER))
				{
					m_strConfig += L"IsInstaller=\"yes\"\r\n";
					GetDlgItemTextW(IDE_RUN_PROGRAM, strText);
					if (strText.Length())
						m_strConfig += L"RunProgram=\"" + strText + L"\"\r\n";
					GetDlgItemTextW(IDE_DIRECTORY, strText);
					if (strText.Length())
						m_strConfig += L"Directory=\"" + strText + L"\"\r\n";
					GetDlgItemTextW(IDE_EXECUTE_FILE, strText);
					if (strText.Length())
						m_strConfig += L"ExecuteFile=\"" + strText + L"\"\r\n";
					GetDlgItemTextW(IDE_EXECUTE_PARAMETERS, strText);
					if (strText.Length())
						m_strConfig += L"ExecuteParameters=\"" + strText + L"\"\r\n";
					m_strConfig += L"Progress=\"";
					m_strConfig += ::IsDlgButtonChecked(m_hWnd, IDC_PROGRESS) ? L"yes" : L"no";
					m_strConfig += L"\"\r\n";
				}
				m_strConfig += L";!@InstallEnd@!";
			}
			// 以下IDCANCELと同じ処理なのでbreakは無し
		case IDCANCEL:
			::EndDialog(m_hWnd, LOWORD(wParam));
			break;
		case IDC_IS_INSTALLER:
			{
				BOOL bIsChecked = ::IsDlgButtonChecked(m_hWnd, IDC_IS_INSTALLER);
				::EnableWindow(::GetDlgItem(m_hWnd, IDE_RUN_PROGRAM),  bIsChecked);
				::EnableWindow(::GetDlgItem(m_hWnd, IDE_DIRECTORY), bIsChecked);
				::EnableWindow(::GetDlgItem(m_hWnd, IDE_EXECUTE_FILE), bIsChecked);
				::EnableWindow(::GetDlgItem(m_hWnd, IDE_EXECUTE_PARAMETERS), bIsChecked);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROGRESS), bIsChecked);
				break;
			}
		default:
			return FALSE;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
