// 7zTestDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "7zTest.h"
#include "7zTestDlg.h"
#include <locale.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct ARCHIVER_API
{
	const char* lpName;
	UINT nID;
};

static const char* lpDllName[] = { "7-zip32.dll", "Unlha32.dll", "Unarj32j.dll", "Unzip32.dll", "Zip32j.dll", "Tar32.dll", "Cab32.dll", "Unrar32.dll", "Bga32.dll", "Ungca32.dll", "yz1.dll" };
static const char* lpPrefix[] = {"SevenZip", "Unlha", "Unarj", "UnZip", "UnZip", "Tar", "Cab", "Unrar", "Bga", "UnGCA", "Yz1" };
static const UINT wm_arcextract = ::RegisterWindowMessage(WM_ARCEXTRACT);
static const ARCHIVER_API api[] =
{
	{ "Command", IDC_COMMAND },
	{ "GetVersion",IDC_GET_VERSION },
	{ "GetRunning",IDC_GET_RUNNING },
	{ "ConfigDialog",IDC_CONFIG_DIALOG },
	{ "CheckArchive",IDC_CHECK_ARCHIVE },
	{ "FunctionList",IDC_QUERY_FUNCTION_LIST },
	{ "GetFileCount",IDC_GET_FILE_COUNT },
	{ "OpenArchive",IDC_OPEN_ARCHIVE },
	{ "FindFirst",IDC_FIND_FIRST },
	{ "FindNext",IDC_FIND_NEXT },
	{ "CloseArchive",IDC_CLOSE_ARCHIVE },
	{ "GetArcFileName",IDC_GET_ARC_FILE_NAME },
	{ "GetArcFileSize",IDC_GET_ARC_FILE_SIZE },
	{ "GetArcOriginalSize",IDC_GET_ARC_ORIGINAL_SIZE },
	{ "GetArcCompressedSize",IDC_GET_ARC_COMPRESSED_SIZE },
	{ "GetArcRatio",IDC_GET_ARC_RATIO },
	{ "GetArcDate",IDC_GET_ARC_DATE },
	{ "GetArcTime",IDC_GET_ARC_TIME },
	{ "GetArcOSType",IDC_GET_ARC_OS_TYPE },
	{ "IsSFXFile",IDC_IS_SFX_FILE },
	{ "GetFileName",IDC_GET_FILE_NAME },
	{ "GetOriginalSize",IDC_GET_ORIGINAL_SIZE },
	{ "GetCmpSize",IDC_GET_COMPRESSED_SIZE },
	{ "GetRatio",IDC_GET_RATIO },
	{ "GetDate",IDC_GET_DATE },
	{ "GetTime",IDC_GET_TIME },
	{ "GetCRC",IDC_GET_CRC },
	{ "GetAttribute",IDC_GET_ATTRIBUTE },
	{ "GetOSType",IDC_GET_OS_TYPE },
	{ "GetMethod",IDC_GET_METHOD },
	{ "GetWriteTime",IDC_GET_WRITE_TIME },
	{ "GetWriteTimeEx",IDC_GET_WRITE_TIME_EX },
	{ "GetArcWriteTimeEx",IDC_GET_ARC_WRITE_TIME_EX },
	{ "GetArcCreateTimeEx",IDC_GET_ARC_CREATE_TIME_EX },
	{ "GetArcAccessTimeEx",IDC_GET_ARC_ACCESS_TIME_EX },
	{ "SetOwnerWindow",IDC_SET_OWNER_WINDOW },
	{ "ClearOwnerWnd",IDC_CLEAR_OWNER_WINDOW },
	{ "SetOwnerWndEx",IDC_SET_OWNER_WINDOW_EX },
	{ "KillOwnerWndEx",IDC_KILL_OWNER_WINDOW_EX },
	{ "GetSubVersion",IDC_GET_SUB_VERSION },
	{ "GetArcFileSizeEx",IDC_GET_ARC_FILE_SIZE_EX },
	{ "GetArcOriginalSizeEx",IDC_GET_ARC_ORIGINAL_SIZE_EX },
	{ "GetArcCompressedSizeEx",IDC_GET_ARC_COMPRESSED_SIZE_EX },
	{ "GetOriginalSizeEx",IDC_GET_ORIGINAL_SIZE_EX },
	{ "GetCmpSizeEx",IDC_GET_COMPRESSED_SIZE_EX },
	{ "GetCreateTime",IDC_GET_CREATE_TIME },
	{ "GetAccessTime",IDC_GET_ACCESS_TIME },
	{ "GetArchiveType",IDC_GET_ARCHIVE_TYPE },
	{ "SetOwnWndEx64",IDC_SET_OWNER_WINDOW_EX64 },
	{ "KillOwnWndEx64",IDC_KILL_OWNER_WINDOW_EX },
	{ "GetCreateTimeEx",IDC_GET_CREATE_TIME_EX },
	{ "GetAccessTimeEx",IDC_GET_ACCESS_TIME_EX },
	{ "SetUnicodeMode",IDC_SET_UNICODE_MODE },
	{ "SetDefaultPassword",IDC_SET_DEFAULT_PASSWORD },
	{ "GetDefaultPassword",IDC_GET_DEFAULT_PASSWORD },
	{ "PasswordDialog",IDC_PASSWORD_DIALOG },
	{ "SetPriority", IDC_SET_PRIORYTY },
	{ "SfxConfigDialog", IDC_SFX_CONFIG_DIALOG },
	{ "SfxFileStoring", IDC_SFX_FILE_STORING },
	{ "GetLastError", IDC_GET_LAST_ERROR }
};

#define IF_PROCEDURE_NOT_FOUND(x)	if (!x) { SetDlgItemText(IDS_RESULT, "非サポートの関数です。"); return; }

/////////////////////////////////////////////////////////////////////////////
// CMy7zTestDlg ダイアログ

CMy7zTestDlg::CMy7zTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy7zTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy7zTestDlg)
	m_strInput = _T("");
	m_strOutput = _T("");
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hArcDll= NULL;
	m_hArc = NULL;
}

void CMy7zTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy7zTestDlg)
	DDX_Control(pDX, IDC_COMMAND, m_button);
	DDX_Control(pDX, IDC_API, m_cmbApi);
	DDX_Control(pDX, IDC_DLL_LIST, m_cmbDllList);
	DDX_Control(pDX, IDL_LIST, m_lcList);
	DDX_CBString(pDX, IDC_INPUT, m_strInput);
	DDX_Text(pDX, IDE_OUTPUT, m_strOutput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy7zTestDlg, CDialog)
	//{{AFX_MSG_MAP(CMy7zTestDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_DLL_LIST, OnSelchangeDllList)
	ON_CBN_SELCHANGE(IDC_API, OnSelchangeApi)
	ON_BN_CLICKED(IDC_COMMAND, OnSevenZip)
	ON_BN_CLICKED(IDC_OPEN_ARCHIVE, OnOpenArchive)
	ON_BN_CLICKED(IDC_FIND_FIRST, OnFindFirst)
	ON_BN_CLICKED(IDC_FIND_NEXT, OnFindNext)
	ON_BN_CLICKED(IDC_CLOSE_ARCHIVE, OnCloseArchive)
	ON_BN_CLICKED(IDC_GET_FILE_COUNT, OnGetFileCount)
	ON_BN_CLICKED(IDC_QUERY_FUNCTION_LIST, OnQueryFunctionList)
	ON_BN_CLICKED(IDC_GET_VERSION, OnGetVersion)
	ON_BN_CLICKED(IDC_GET_RUNNING, OnGetRunning)
	ON_BN_CLICKED(IDC_CONFIG_DIALOG, OnConfigDialog)
	ON_BN_CLICKED(IDC_CHECK_ARCHIVE, OnCheckArchive)
	ON_BN_CLICKED(IDC_GET_ARC_FILE_NAME, OnGetArcFileName)
	ON_BN_CLICKED(IDC_GET_ARC_FILE_SIZE, OnGetArcFileSize)
	ON_BN_CLICKED(IDC_GET_ARC_ORIGINAL_SIZE, OnGetArcOriginalSize)
	ON_BN_CLICKED(IDC_GET_ARC_COMPRESSED_SIZE, OnGetArcCompressedSize)
	ON_BN_CLICKED(IDC_GET_ARC_RATIO, OnGetArcRatio)
	ON_BN_CLICKED(IDC_GET_ARC_DATE, OnGetArcDate)
	ON_BN_CLICKED(IDC_GET_ARC_TIME, OnGetArcTime)
	ON_BN_CLICKED(IDC_GET_ARC_OS_TYPE, OnGetArcOSType)
	ON_BN_CLICKED(IDC_IS_SFX_FILE, OnIsSFXFile)
	ON_BN_CLICKED(IDC_GET_FILE_NAME, OnGetFileName)
	ON_BN_CLICKED(IDC_GET_ORIGINAL_SIZE, OnGetOriginalSize)
	ON_BN_CLICKED(IDC_GET_COMPRESSED_SIZE, OnGetCompressedSize)
	ON_BN_CLICKED(IDC_GET_RATIO, OnGetRatio)
	ON_BN_CLICKED(IDC_GET_DATE, OnGetDate)
	ON_BN_CLICKED(IDC_GET_TIME, OnGetTime)
	ON_BN_CLICKED(IDC_GET_CRC, OnGetCRC)
	ON_BN_CLICKED(IDC_GET_ATTRIBUTE, OnGetAttribute)
	ON_BN_CLICKED(IDC_GET_OS_TYPE, OnGetOSType)
	ON_BN_CLICKED(IDC_GET_METHOD, OnGetMethod)
	ON_BN_CLICKED(IDC_GET_WRITE_TIME, OnGetWriteTime)
	ON_BN_CLICKED(IDC_GET_WRITE_TIME_EX, OnGetWriteTimeEx)
	ON_BN_CLICKED(IDC_GET_ARC_WRITE_TIME_EX, OnGetArcWriteTimeEx)
	ON_BN_CLICKED(IDC_GET_ARC_CREATE_TIME_EX, OnGetArcCreateTimeEx)
	ON_BN_CLICKED(IDC_GET_ARC_ACCESS_TIME_EX, OnGetArcAccessTimeEx)
	ON_BN_CLICKED(IDC_SET_OWNER_WINDOW, OnSetOwnerWindow)
	ON_BN_CLICKED(IDC_CLEAR_OWNER_WINDOW, OnClearOwnerWindow)
	ON_BN_CLICKED(IDC_SET_OWNER_WINDOW_EX, OnSetOwnerWindowEx)
	ON_BN_CLICKED(IDC_KILL_OWNER_WINDOW_EX, OnKillOwnerWindowEx)
	ON_BN_CLICKED(IDC_SET_OWNER_WINDOW_EX64, OnSetOwnerWindowEx64)
	ON_BN_CLICKED(IDC_KILL_OWNER_WINDOW_EX64, OnKillOwnerWindowEx64)
	ON_BN_CLICKED(IDC_GET_SUB_VERSION, OnGetSubVersion)
	ON_BN_CLICKED(IDC_GET_ARC_FILE_SIZE_EX, OnGetArcFileSizeEx)
	ON_BN_CLICKED(IDC_GET_ARC_ORIGINAL_SIZE_EX, OnGetArcOriginalSizeEx)
	ON_BN_CLICKED(IDC_GET_ARC_COMPRESSED_SIZE_EX, OnGetArcCompressedSizeEx)
	ON_BN_CLICKED(IDC_GET_ORIGINAL_SIZE_EX, OnGetOriginalSizeEx)
	ON_BN_CLICKED(IDC_GET_COMPRESSED_SIZE_EX, OnGetCompressedSizeEx)
	ON_BN_CLICKED(IDC_GET_CREATE_TIME, OnGetCreateTime)
	ON_BN_CLICKED(IDC_GET_ACCESS_TIME, OnGetAccessTime)
	ON_BN_CLICKED(IDC_GET_ARCHIVE_TYPE, OnGetArchiveType)
	ON_BN_CLICKED(IDC_GET_CREATE_TIME_EX, OnGetCreateTimeEx)
	ON_BN_CLICKED(IDC_GET_ACCESS_TIME_EX, OnGetAccessTimeEx)
	ON_BN_CLICKED(IDC_SET_UNICODE_MODE, OnSetUnicodeMode)
	ON_BN_CLICKED(IDC_SET_DEFAULT_PASSWORD, OnSetDefaultPassword)
	ON_BN_CLICKED(IDC_GET_DEFAULT_PASSWORD, OnGetDefaultPassword)
	ON_BN_CLICKED(IDC_PASSWORD_DIALOG, OnPasswordDialog)
	ON_BN_CLICKED(IDC_SET_PRIORYTY, OnSetPriority)
	ON_BN_CLICKED(IDC_SFX_CONFIG_DIALOG, OnSfxConfigDialog)
	ON_BN_CLICKED(IDC_SFX_FILE_STORING, OnSfxFileStoring)
	ON_BN_CLICKED(IDC_GET_LAST_ERROR, OnGetLastError)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(wm_arcextract, OnArcExtract)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy7zTestDlg メッセージ ハンドラ

BOOL CMy7zTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	for (int i = 0; i < sizeof(lpDllName) / sizeof(char*); ++i)
		m_cmbDllList.AddString(lpDllName[i]);
	m_cmbDllList.SetCurSel(0);
	OnSelchangeDllList();
	for (i = 0; i < sizeof(api) / sizeof(ARCHIVER_API); ++i)
		m_cmbApi.SetItemData(m_cmbApi.AddString(api[i].lpName), api[i].nID);
	m_cmbApi.SetCurSel(0);

	m_lcList.InsertColumn(1, _T("ファイル名"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(2, _T("サイズ"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(3, _T("格納サイズ"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(4, _T("圧縮率"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(5, _T("更新日時"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(6, _T("属性"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(7, _T("CRC"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(8, _T("MODE"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(9, _T("OSType"), LVCFMT_LEFT, 50, -1);
	m_lcList.InsertColumn(10, _T("Flag"), LVCFMT_LEFT, 50, -1);

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CMy7zTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CMy7zTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CMy7zTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	::FreeLibrary(m_hArcDll);
}

void CMy7zTestDlg::OnSelchangeDllList() 
{
	if (m_hArcDll)
		::FreeLibrary(m_hArcDll);
	m_nSelectDll = m_cmbDllList.GetCurSel();
	m_hArcDll = ::LoadLibrary(lpDllName[m_nSelectDll]);
	SetDlgItemInt(IDS_RESULT, (int)m_hArcDll);
}

void CMy7zTestDlg::OnSelchangeApi() 
{
	m_button.SetDlgCtrlID(m_cmbApi.GetItemData(m_cmbApi.GetCurSel()));
}


FARPROC CMy7zTestDlg::GetFuncAddress(LPCTSTR lpFuncName)
{
	char lpApiName[256];
	sprintf(lpApiName, "%s%s", lpPrefix[m_nSelectDll], lpFuncName);
	return ::GetProcAddress(m_hArcDll, lpApiName);
}

void CMy7zTestDlg::OnSevenZip() 
{
	UpdateData();
	LPSTR lpMes = m_strOutput.GetBuffer(65536);
	FARPROC f = ::GetProcAddress(m_hArcDll, lpPrefix[m_nSelectDll]);
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int  (WINAPI* SEVEN_ZIP)(const HWND,LPCSTR,LPSTR,const DWORD);
	int nRes = ((SEVEN_ZIP)f)( m_hWnd, (char*)(LPCTSTR)m_strInput, lpMes, 65536);
	m_strOutput.ReleaseBuffer();
	if (nRes < 0x8000)
		SetDlgItemInt(IDS_RESULT, nRes);
	else
	{
		char lpRes[16];
		sprintf(lpRes, "0x%x", nRes);
		SetDlgItemText(IDS_RESULT, lpRes);
	}
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnOpenArchive() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("OpenArchive");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef HARC (WINAPI* OPEN_ARCHIVE)(const HWND, LPCSTR, const DWORD);
	m_hArc = ((OPEN_ARCHIVE)f)(m_hWnd, (char*)(LPCTSTR)m_strInput, 0);
	SetDlgItemInt(IDS_RESULT, (int)m_hArc);
}

void CMy7zTestDlg::OnFindFirst() 
{
	UpdateData();
	INDIVIDUALINFO indiv;
	FARPROC f = GetFuncAddress("FindFirst");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI* FIND_FIRST)(HARC ,LPCSTR ,INDIVIDUALINFO*);
	int nRes = ((FIND_FIRST)f)(m_hArc, (char*)(LPCTSTR)m_strInput, &indiv);
	if (nRes == 0)
		AddList(&indiv);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnFindNext() 
{
	UpdateData();
	INDIVIDUALINFO indiv;
	FARPROC f = GetFuncAddress("FindNext");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI* FIND_NEXT)(HARC ,INDIVIDUALINFO*);
	int nRes = ((FIND_NEXT)f)(m_hArc, &indiv);
	if (nRes == 0)
		AddList(&indiv);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::AddList(INDIVIDUALINFO *lpSubInfo)
{
	int nCount = m_lcList.GetItemCount();
	char lpText[256];
	m_lcList.InsertItem(nCount, lpSubInfo->szFileName);
	sprintf(lpText, "%u", lpSubInfo->dwOriginalSize);
	m_lcList.SetItemText(nCount, 1, lpText);
	sprintf(lpText, "%u", lpSubInfo->dwCompressedSize);
	m_lcList.SetItemText(nCount, 2, lpText);
	if (lpSubInfo->wRatio == 0xFFFF)
		strcpy(lpText, "?");
	else
		sprintf(lpText, "%u.%u%", lpSubInfo->wRatio / 10, lpSubInfo->wRatio % 10);
	m_lcList.SetItemText(nCount, 3, lpText);
	sprintf(lpText,
			"%04u-%02u-%02u %02u:%02u:%02u",
			(lpSubInfo->wDate >> 9) + 1980,
			(lpSubInfo->wDate >> 5) & 0x0F,
			(lpSubInfo->wDate & 0x1F),
			(lpSubInfo->wTime >> 11),
			(lpSubInfo->wTime >> 5) & 0x3F,
			(lpSubInfo->wTime & 0x1F) * 2);
	m_lcList.SetItemText(nCount, 4, lpText);
	m_lcList.SetItemText(nCount, 5, lpSubInfo->szAttribute);
	sprintf(lpText, "%08x", lpSubInfo->dwCRC);
	m_lcList.SetItemText(nCount, 6, lpText);
	m_lcList.SetItemText(nCount, 7, lpSubInfo->szMode);
	sprintf(lpText, "%u", lpSubInfo->uOSType);
	m_lcList.SetItemText(nCount, 8, lpText);
	sprintf(lpText, "%u", lpSubInfo->uFlag);
	m_lcList.SetItemText(nCount, 9, lpText);
}

void CMy7zTestDlg::OnCloseArchive() 
{
	FARPROC f = GetFuncAddress("CloseArchive");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * CLOSE_ARCHIVE)(HARC);
	int nRes = ((CLOSE_ARCHIVE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, nRes);
	m_hArc = NULL;
}

void CMy7zTestDlg::OnGetFileCount() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("GetFileCount");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * GET_FILE_COUNT)(LPCSTR);
	int nRes = ((GET_FILE_COUNT)f)((LPCSTR)(LPCTSTR)m_strInput);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnQueryFunctionList() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("QueryFunctionList");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * QUERY_FUNCTION_LIST)(const int);
	BOOL bRes = ((QUERY_FUNCTION_LIST)f)(atoi(m_strInput));
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnGetVersion() 
{
	FARPROC f = GetFuncAddress("GetVersion");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef WORD (WINAPI * GET_VERSION)();
	WORD wRes = ((GET_VERSION)f)();
	SetDlgItemInt(IDS_RESULT, wRes);
}

void CMy7zTestDlg::OnGetRunning() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("GetRunning");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_RUNNING)();
	BOOL bRes = ((GET_RUNNING)f)();
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnConfigDialog() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("ConfigDialog");
	IF_PROCEDURE_NOT_FOUND(f);
	LPSTR _szOptionBuffer= m_strOutput.GetBuffer(1024);
	typedef BOOL (WINAPI * CONFIG_DIALOG)(const HWND, LPSTR, const int);
	BOOL bRes = ((CONFIG_DIALOG)f)(m_hWnd, _szOptionBuffer, atoi(m_strInput));
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
	m_strOutput.ReleaseBuffer();
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnCheckArchive() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("CheckArchive");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * CHECK_ARCHIVE)(LPCSTR, const int);
	BOOL bRes = ((CHECK_ARCHIVE)f)((LPCSTR)(LPCTSTR)m_strInput, 0);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnGetArcFileName() 
{
	FARPROC f = GetFuncAddress("GetArcFileName");
	IF_PROCEDURE_NOT_FOUND(f);
	LPSTR lpFileName = m_strOutput.GetBuffer(FNAME_MAX32);
	typedef int (WINAPI * GET_ARC_FILE_NAME)(HARC, LPSTR, const int);
	int nRes = ((GET_ARC_FILE_NAME)f)(m_hArc, lpFileName, FNAME_MAX32);
	SetDlgItemInt(IDS_RESULT, nRes);
	m_strOutput.ReleaseBuffer();
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnGetArcFileSize() 
{
	FARPROC f = GetFuncAddress("GetArcFileSize");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_ARC_FILE_SIZE)(HARC);
	DWORD dwRes = ((GET_ARC_FILE_SIZE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, dwRes);
}

void CMy7zTestDlg::OnGetArcOriginalSize() 
{
	FARPROC f = GetFuncAddress("GetArcOriginalSize");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_ARC_ORIGINAL_SIZE)(HARC);
	DWORD dwRes = ((GET_ARC_ORIGINAL_SIZE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, dwRes);
}

void CMy7zTestDlg::OnGetArcCompressedSize() 
{
	FARPROC f = GetFuncAddress("GetArcCompressedSize");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_ARC_COMPRESSED_SIZE)(HARC);
	DWORD dwRes = ((GET_ARC_COMPRESSED_SIZE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, dwRes);
}

void CMy7zTestDlg::OnGetArcRatio() 
{
	FARPROC f = GetFuncAddress("GetArcRatio");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef WORD (WINAPI * GET_ARC_RATIO)(HARC);
	WORD wRes = ((GET_ARC_RATIO)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, wRes);
}

void CMy7zTestDlg::OnGetArcDate() 
{
	FARPROC f = GetFuncAddress("GetArcDate");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef WORD (WINAPI * GET_ARC_DATE)(HARC);
	WORD wRes = ((GET_ARC_DATE)f)(m_hArc);
	char lpDate[16];
	sprintf(lpDate, "%.4u-%.2u-%.2u", (wRes >> 9) + 1980, (wRes >> 5) & 0x0F, (wRes & 0x1F));
	SetDlgItemText(IDS_RESULT, lpDate);
}

void CMy7zTestDlg::OnGetArcTime() 
{
	FARPROC f = GetFuncAddress("GetArcTime");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef WORD (WINAPI * GET_ARC_TIME)(HARC);
	WORD wRes = ((GET_ARC_TIME)f)(m_hArc);
	char lpTime[16];
	sprintf(lpTime, "%.2u:%.2u:%.2u", (wRes >> 11), (wRes >> 5) & 0x3F, (wRes & 0x1F) * 2);
	SetDlgItemText(IDS_RESULT, lpTime);
}

void CMy7zTestDlg::OnGetArcOSType() 
{
	FARPROC f = GetFuncAddress("GetArcOSType");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef UINT (WINAPI * GET_ARC_OS_TYPE)(HARC);
	UINT nRes = ((GET_ARC_OS_TYPE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnIsSFXFile() 
{
	FARPROC f = GetFuncAddress("IsSFXFile");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * IS_SFX_FILE)(HARC);
	int nRes = ((IS_SFX_FILE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnGetFileName() 
{
	FARPROC f = GetFuncAddress("GetFileName");
	IF_PROCEDURE_NOT_FOUND(f);
	LPSTR lpBuffer = m_strOutput.GetBuffer(256);
	typedef int (WINAPI * GET_FILE_NAME)(HARC, LPSTR, const int);
	int nRes = ((GET_FILE_NAME)f)(m_hArc, lpBuffer, 256);
	SetDlgItemInt(IDS_RESULT, nRes);
	m_strOutput.ReleaseBuffer();
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnGetOriginalSize() 
{
	FARPROC f = GetFuncAddress("GetOriginalSize");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_ORIGINAL_SIZE)(HARC);
	DWORD dwRes = ((GET_ORIGINAL_SIZE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, dwRes);
}

void CMy7zTestDlg::OnGetCompressedSize() 
{
	FARPROC f = GetFuncAddress("GetCompressedSize");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_COMPRESSED_SIZE)(HARC);
	DWORD dwRes = ((GET_COMPRESSED_SIZE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, dwRes);
}

void CMy7zTestDlg::OnGetRatio() 
{
	FARPROC f = GetFuncAddress("GetRatio");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef WORD (WINAPI * GET_RATIO)(HARC);
	WORD wRes = ((GET_RATIO)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, wRes);
}

void CMy7zTestDlg::OnGetDate() 
{
	FARPROC f = GetFuncAddress("GetDate");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef WORD (WINAPI * GET_DATE)(HARC);
	WORD wRes = ((GET_DATE)f)(m_hArc);
	char lpDate[16];
	sprintf(lpDate, "%.4u-%.2u-%.2u", (wRes >> 9) + 1980, (wRes >> 5) & 0x0F, (wRes & 0x1F));
	SetDlgItemText(IDS_RESULT, lpDate);
}

void CMy7zTestDlg::OnGetTime() 
{
	FARPROC f = GetFuncAddress("GetTime");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef WORD (WINAPI * GET_TIME)(HARC);
	WORD wRes = ((GET_TIME)f)(m_hArc);
	char lpTime[16];
	sprintf(lpTime, "%.2u:%.2u:%.2u", (wRes >> 11), (wRes >> 5) & 0x3F, (wRes & 0x1F) * 2);
	SetDlgItemText(IDS_RESULT, lpTime);
}

void CMy7zTestDlg::OnGetCRC() 
{
	FARPROC f = GetFuncAddress("GetCRC");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_CRC)(HARC);
	DWORD dwRes = ((GET_CRC)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, dwRes);
}

void CMy7zTestDlg::OnGetAttribute() 
{
	FARPROC f = GetFuncAddress("GetAttribute");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * GET_ATTRIBUTE)(HARC);
	int nRes = ((GET_ATTRIBUTE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnGetOSType() 
{
	FARPROC f = GetFuncAddress("GetOSType");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef UINT (WINAPI * GET_OS_TYPE)(HARC);
	UINT nRes = ((GET_OS_TYPE)f)(m_hArc);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnGetMethod() 
{
	FARPROC f = GetFuncAddress("GetMethod");
	IF_PROCEDURE_NOT_FOUND(f);
	LPSTR lpString = m_strOutput.GetBuffer(128);
	typedef int (WINAPI * GET_GET_METHOD)(HARC, LPSTR, const int);
	int nRes = ((GET_GET_METHOD)f)(m_hArc, lpString, 128);
	SetDlgItemInt(IDS_RESULT, nRes);
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnGetWriteTime() 
{
	FARPROC f = GetFuncAddress("GetWriteTime");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_WRITE_TIME)(HARC);
	DWORD dwRes = ((GET_WRITE_TIME)f)(m_hArc);
	SetDlgItemText(IDS_RESULT, ctime((time_t*)&dwRes));
}

void CMy7zTestDlg::OnGetWriteTimeEx() 
{
	FILETIME ft;
	FARPROC f = GetFuncAddress("GetWriteTimeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_WRITE_TIME_EX)(HARC, FILETIME*);
	BOOL bRes = ((GET_WRITE_TIME_EX)f)(m_hArc, &ft);
	CTime t(ft);
	SetDlgItemText(IDS_RESULT, bRes ? t.Format("%Y-%m-%d %H:%M:%S") : "エラー");
}

void CMy7zTestDlg::OnGetCreateTimeEx() 
{
	FILETIME ft;
	FARPROC f = GetFuncAddress("GetCreateTimeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_CREATE_TIME_EX)(HARC, FILETIME*);
	BOOL bRes = ((GET_CREATE_TIME_EX)f)(m_hArc, &ft);
	CTime t(ft);
	SetDlgItemText(IDS_RESULT, bRes ? t.Format("%Y-%m-%d %H:%M:%S") : "エラー");
}

void CMy7zTestDlg::OnGetAccessTimeEx() 
{
	FILETIME ft;
	FARPROC f = GetFuncAddress("GetAccessTimeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_ACCESS_TIME_EX)(HARC, FILETIME*);
	BOOL bRes = ((GET_ACCESS_TIME_EX)f)(m_hArc, &ft);
	CTime t(ft);
	SetDlgItemText(IDS_RESULT, bRes ? t.Format("%Y-%m-%d %H:%M:%S") : "エラー");
}

void CMy7zTestDlg::OnGetArcWriteTimeEx() 
{
	FILETIME ft;
	FARPROC f = GetFuncAddress("GetArcWriteTimeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_ARC_WRITE_TIME_EX)(HARC, FILETIME*);
	BOOL bRes = ((GET_ARC_WRITE_TIME_EX)f)(m_hArc, &ft);
	CTime t(ft);
	SetDlgItemText(IDS_RESULT, t.Format("%Y-%m-%d %H:%M:%S"));
}

void CMy7zTestDlg::OnGetArcCreateTimeEx() 
{
	FILETIME ft;
	FARPROC f = GetFuncAddress("GetArcCreateTimeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_ARC_CREATE_TIME_EX)(HARC, FILETIME*);
	BOOL bRes = ((GET_ARC_CREATE_TIME_EX)f)(m_hArc, &ft);
	CTime t(ft);
	SetDlgItemText(IDS_RESULT, t.Format("%Y-%m-%d %H:%M:%S"));
}

void CMy7zTestDlg::OnGetArcAccessTimeEx() 
{
	FILETIME ft;
	FARPROC f = GetFuncAddress("GetArcAccessTimeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_ARC_ACCESS_TIME_EX)(HARC, FILETIME*);
	BOOL bRes = ((GET_ARC_ACCESS_TIME_EX)f)(m_hArc, &ft);
	CTime t(ft);
	SetDlgItemText(IDS_RESULT, t.Format("%Y-%m-%d %H:%M:%S"));
}

LRESULT CMy7zTestDlg::OnArcExtract(WPARAM wParam, LPARAM lParam)
{
	LPEXTRACTINGINFO lpEai = (LPEXTRACTINGINFO)lParam;
	TRACE("state=%d", wParam);
	if (lpEai)
		TRACE(" fsize=%10d wsize=%10d sname=%-30s dname=%s", lpEai->dwFileSize, lpEai->dwWriteSize, lpEai->szSourceFileName, lpEai->szDestFileName);
	TRACE("\n");
	return 0;
}

BOOL CALLBACK ArchiveProc(HWND _hwnd, UINT _uMsg, UINT _nState, LPVOID _lpEis)
{
	LPEXTRACTINGINFOEX lpEiex = (LPEXTRACTINGINFOEX)_lpEis;
	TRACE("state=%d", _nState);
	if (lpEiex)
		TRACE(" fsize=%10d wsize=%10d csize=%10d crc=%.8x os=%d raito=%5d date=%.4u-%.2u-%.2u time=%.2u:%.2u:%.2u attr=%-8s mode=%-8s sname=%-30s dname=%s", lpEiex->exinfo.dwFileSize, lpEiex->exinfo.dwWriteSize, lpEiex->dwCompressedSize, lpEiex->dwCRC, lpEiex->uOSType, lpEiex->wRatio, (lpEiex->wDate >> 9) + 1980, (lpEiex->wDate >> 5) & 0x0F, (lpEiex->wDate & 0x1F), (lpEiex->wTime >> 11), (lpEiex->wTime >> 5) & 0x3F, (lpEiex->wTime & 0x1F) * 2, lpEiex->szAttribute, lpEiex->szMode, lpEiex->exinfo.szSourceFileName, lpEiex->exinfo.szDestFileName);
	TRACE("\n");
	return TRUE;
}

BOOL CALLBACK ArchiveProc32(HWND _hwnd, UINT _uMsg, UINT _nState, LPVOID _lpEis)
{
	LPEXTRACTINGINFOEX32 lpEiex = (LPEXTRACTINGINFOEX32)_lpEis;
	TRACE("state=%d", _nState);
	if (lpEiex)
	{
		CTime ct(lpEiex->ftCreateTime);
		CTime at(lpEiex->ftAccessTime);
		CTime wt(lpEiex->ftWriteTime);
		TRACE(" fsize=%10d wsize=%10d csize=%10d attr=%3d crc=%.8x os=%d raito=%5d ctime=%s atime=%s wtime=%s mode=%-8s sname=%-30s dname=%s", lpEiex->dwFileSize, lpEiex->dwWriteSize, lpEiex->dwCompressedSize, lpEiex->dwAttributes, lpEiex->dwCRC, lpEiex->uOSType, lpEiex->wRatio, ct.Format("%Y-%m-%d %H:%M:%S"), at.Format("%Y-%m-%d %H:%M:%S"), wt.Format("%Y-%m-%d %H:%M:%S"), lpEiex->szMode, lpEiex->exinfo.szSourceFileName, lpEiex->exinfo.szDestFileName);
	}
	TRACE("\n");
	return TRUE;
}

BOOL CALLBACK ArchiveProc64(HWND _hwnd, UINT _uMsg, UINT _nState, LPVOID _lpEis)
{
	LPEXTRACTINGINFOEX64 lpEiex = (LPEXTRACTINGINFOEX64)_lpEis;
	TRACE("msg=%d state=%d", _uMsg, _nState);
	if (lpEiex)
	{
		CTime ct(lpEiex->ftCreateTime);
		CTime at(lpEiex->ftAccessTime);
		CTime wt(lpEiex->ftWriteTime);
		TRACE(" fsize=%10I64d wsize=%10I64d csize=%10I64d attr=%3d crc=%.8x os=%d raito=%5d ctime=%s atime=%s wtime=%s mode=%-8s sname=%-30s dname=%s", lpEiex->llFileSize, lpEiex->llWriteSize, lpEiex->llCompressedSize, lpEiex->dwAttributes, lpEiex->dwCRC, lpEiex->uOSType, lpEiex->wRatio, ct.Format("%Y-%m-%d %H:%M:%S"), at.Format("%Y-%m-%d %H:%M:%S"), wt.Format("%Y-%m-%d %H:%M:%S"), lpEiex->szMode, lpEiex->exinfo.szSourceFileName, lpEiex->exinfo.szDestFileName);
	}
	TRACE("\n");
	return TRUE;
}

void CMy7zTestDlg::OnSetOwnerWindow() 
{
	FARPROC f = GetFuncAddress("SetOwnerWindow");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * SET_OWNER_WINDOW)(HWND);
	BOOL bRes = ((SET_OWNER_WINDOW)f)(m_hWnd);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnClearOwnerWindow() 
{
	FARPROC f = GetFuncAddress("ClearOwnerWindow");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * CLEAR_OWNER_WINDOW)();
	BOOL bRes = ((CLEAR_OWNER_WINDOW)f)();
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnSetOwnerWindowEx() 
{
	FARPROC f = GetFuncAddress("SetOwnerWindowEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * SET_OWNER_WINDOW_EX)(HWND, LPARCHIVERPROC);
	BOOL bRes = ((SET_OWNER_WINDOW_EX)f)(m_hWnd, ArchiveProc);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnKillOwnerWindowEx() 
{
	FARPROC f = GetFuncAddress("KillOwnerWindowEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * KILL_OWNER_WINDOW_EX)(HWND);
	BOOL bRes = ((KILL_OWNER_WINDOW_EX)f)(m_hWnd);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnSetOwnerWindowEx64() 
{
	FARPROC f = GetFuncAddress("SetOwnerWindowEx64");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * SET_OWNER_WINDOW_EX64)(HWND, LPARCHIVERPROC, DWORD);
	BOOL bRes = ((SET_OWNER_WINDOW_EX64)f)(m_hWnd, ArchiveProc64, sizeof(EXTRACTINGINFOEX64));
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnKillOwnerWindowEx64() 
{
	FARPROC f = GetFuncAddress("KillOwnerWindowEx64");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * KILL_OWNER_WINDOW_EX64)(HWND);
	BOOL bRes = ((KILL_OWNER_WINDOW_EX64)f)(m_hWnd);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnGetSubVersion() 
{
	FARPROC f = GetFuncAddress("GetSubVersion");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef WORD (WINAPI * GET_SUB_VERSION)();
	WORD wRes = ((GET_SUB_VERSION)f)();
	SetDlgItemInt(IDS_RESULT, wRes);
}

void CMy7zTestDlg::OnGetArcFileSizeEx() 
{
	ULHA_INT64 llSize;
	FARPROC f = GetFuncAddress("GetArcFileSizeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_ARC_FILE_SIZE_EX)(HARC, ULHA_INT64*);
	BOOL bRes = ((GET_ARC_FILE_SIZE_EX)f)(m_hArc, &llSize);
	m_strOutput.Format("%I64u", llSize);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnGetArcOriginalSizeEx() 
{
	ULHA_INT64 llSize;
	FARPROC f = GetFuncAddress("GetArcOriginalSizeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_ARC_ORIGINAL_SIZE_EX)(HARC, ULHA_INT64*);
	BOOL bRes = ((GET_ARC_ORIGINAL_SIZE_EX)f)(m_hArc, &llSize);
	m_strOutput.Format("%I64u", llSize);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnGetArcCompressedSizeEx() 
{
	ULHA_INT64 llSize;
	FARPROC f = GetFuncAddress("GetArcCompressedSizeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_ARC_COMPRESSED_SIZE_EX)(HARC, ULHA_INT64*);
	BOOL bRes = ((GET_ARC_COMPRESSED_SIZE_EX)f)(m_hArc, &llSize);
	m_strOutput.Format("%I64u", llSize);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnGetOriginalSizeEx() 
{
	ULHA_INT64 llSize;
	FARPROC f = GetFuncAddress("GetOriginalSizeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_ORIGINAL_SIZE_EX)(HARC, ULHA_INT64*);
	BOOL bRes = ((GET_ORIGINAL_SIZE_EX)f)(m_hArc, &llSize);
	m_strOutput.Format("%I64u", llSize);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnGetCompressedSizeEx() 
{
	ULHA_INT64 llSize;
	FARPROC f = GetFuncAddress("GetCompressedSizeEx");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * GET_COMPRESSED_SIZE_EX)(HARC, ULHA_INT64*);
	BOOL bRes = ((GET_COMPRESSED_SIZE_EX)f)(m_hArc, &llSize);
	m_strOutput.Format("%I64u", llSize);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
	UpdateData(FALSE);
}

void CMy7zTestDlg::OnGetCreateTime() 
{
	FARPROC f = GetFuncAddress("GetCreateTime");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_CREATE_TIME)(HARC);
	DWORD dwRes = ((GET_CREATE_TIME)f)(m_hArc);
	SetDlgItemText(IDS_RESULT, ctime((time_t*)&dwRes));
}

void CMy7zTestDlg::OnGetAccessTime() 
{
	FARPROC f = GetFuncAddress("GetAccessTime");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef DWORD (WINAPI * GET_ACCESS_TIME)(HARC);
	DWORD dwRes = ((GET_ACCESS_TIME)f)(m_hArc);
	SetDlgItemText(IDS_RESULT, ctime((time_t*)&dwRes));
}

void CMy7zTestDlg::OnGetArchiveType() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("GetArchiveType");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * GET_ARCHIVE_TYPE)(LPCSTR);
	int nRes = ((GET_ARCHIVE_TYPE)f)(m_strInput);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnSetUnicodeMode() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("SetUnicodeMode");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * SET_UNICODE_MODE)(BOOL);
	BOOL bRes = ((SET_UNICODE_MODE)f)(atoi(m_strInput));
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnSetDefaultPassword() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("SetDefaultPassword");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * SET_DEFAULT_PASSWORD)(HARC, LPCSTR);
	int nRes = ((SET_DEFAULT_PASSWORD)f)(m_hArc, m_strInput);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnGetDefaultPassword() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("GetDefaultPassword");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * GET_DEFAULT_PASSWORD)(HARC, LPSTR, DWORD);
	DWORD dwRes = ((GET_DEFAULT_PASSWORD)f)(m_hArc, m_strOutput.GetBuffer(64), 64);
	m_strOutput.ReleaseBuffer();
	UpdateData(FALSE);
	SetDlgItemInt(IDS_RESULT, dwRes);
}

void CMy7zTestDlg::OnPasswordDialog() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("PasswordDialog");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * PASSWORD_DIALOG)(HWND, LPSTR, DWORD);
	int nRes = ((PASSWORD_DIALOG)f)(m_hWnd, m_strOutput.GetBuffer(8), 8);
	m_strOutput.ReleaseBuffer();
	UpdateData(FALSE);
	SetDlgItemInt(IDS_RESULT, nRes);
}
void CMy7zTestDlg::OnSetPriority() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("SetPriority");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * SET_PRIORITY)(int);
	BOOL bRes = ((SET_PRIORITY)f)(atoi(m_strInput));
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnSfxConfigDialog() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("SfxConfigDialog");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * SFX_CONFIG_DIALOG)(HWND, LPSTR, DWORD);
	int nRes = ((SFX_CONFIG_DIALOG)f)(m_hWnd, m_strOutput.GetBuffer(4096), 4096);
	m_strOutput.ReleaseBuffer();
	UpdateData(FALSE);
	SetDlgItemInt(IDS_RESULT, nRes);
}

void CMy7zTestDlg::OnSfxFileStoring() 
{
	UpdateData();
	FARPROC f = GetFuncAddress("SfxFileStoring");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef BOOL (WINAPI * SFX_FILE_STORING)(LPCSTR);
	BOOL bRes = ((SFX_FILE_STORING)f)(m_strInput);
	SetDlgItemText(IDS_RESULT, bRes ? "TRUE" : "FALSE");
}

void CMy7zTestDlg::OnGetLastError() 
{
	DWORD dwError = 0;
	UpdateData();
	FARPROC f = GetFuncAddress("GetLastError");
	IF_PROCEDURE_NOT_FOUND(f);
	typedef int (WINAPI * GET_LAST_ERROR)(LPDWORD);
	int nRes = ((GET_LAST_ERROR)f)(&dwError);
	char lpsz[1024];
	::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpsz, 1024, NULL);
	SetDlgItemText(IDE_OUTPUT, lpsz);
	if (nRes < 0x8000)
		SetDlgItemInt(IDS_RESULT, nRes);
	else
	{
		char lpRes[16];
		sprintf(lpRes, "0x%x", nRes);
		SetDlgItemText(IDS_RESULT, lpRes);
	}
}
