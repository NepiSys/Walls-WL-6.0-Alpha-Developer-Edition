// 7zTestDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_7ZTESTDLG_H__74AF8B5B_EAFA_4EEA_9A08_26D0A4ED97E7__INCLUDED_)
#define AFX_7ZTESTDLG_H__74AF8B5B_EAFA_4EEA_9A08_26D0A4ED97E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy7zTestDlg �_�C�A���O

class CMy7zTestDlg : public CDialog
{
// �\�z
public:
	CMy7zTestDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMy7zTestDlg)
	enum { IDD = IDD_MY7ZTEST_DIALOG };
	CButton	m_button;
	CComboBox	m_cmbApi;
	CComboBox	m_cmbDllList;
	CListCtrl	m_lcList;
	CString	m_strInput;
	CString	m_strOutput;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMy7zTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	int m_nSelectDll;
	FARPROC GetFuncAddress(LPCTSTR lpFuncName);
	void AddList(INDIVIDUALINFO *lpSubInfo);
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMy7zTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeDllList();
	afx_msg void OnSelchangeApi();
	afx_msg void OnSevenZip();
	afx_msg void OnOpenArchive();
	afx_msg void OnFindFirst();
	afx_msg void OnFindNext();
	afx_msg void OnCloseArchive();
	afx_msg void OnGetFileCount();
	afx_msg void OnQueryFunctionList();
	afx_msg void OnGetVersion();
	afx_msg void OnGetRunning();
	afx_msg void OnConfigDialog();
	afx_msg void OnCheckArchive();
	afx_msg void OnGetArcFileName();
	afx_msg void OnGetArcFileSize();
	afx_msg void OnGetArcOriginalSize();
	afx_msg void OnGetArcCompressedSize();
	afx_msg void OnGetArcRatio();
	afx_msg void OnGetArcDate();
	afx_msg void OnGetArcTime();
	afx_msg void OnGetArcOSType();
	afx_msg void OnIsSFXFile();
	afx_msg void OnGetFileName();
	afx_msg void OnGetOriginalSize();
	afx_msg void OnGetCompressedSize();
	afx_msg void OnGetRatio();
	afx_msg void OnGetDate();
	afx_msg void OnGetTime();
	afx_msg void OnGetCRC();
	afx_msg void OnGetAttribute();
	afx_msg void OnGetOSType();
	afx_msg void OnGetMethod();
	afx_msg void OnGetWriteTime();
	afx_msg void OnGetWriteTimeEx();
	afx_msg void OnGetArcWriteTimeEx();
	afx_msg void OnGetArcCreateTimeEx();
	afx_msg void OnGetArcAccessTimeEx();
	afx_msg void OnSetOwnerWindow();
	afx_msg void OnClearOwnerWindow();
	afx_msg void OnSetOwnerWindowEx();
	afx_msg void OnKillOwnerWindowEx();
	afx_msg void OnSetOwnerWindowEx64();
	afx_msg void OnKillOwnerWindowEx64();
	afx_msg void OnGetSubVersion();
	afx_msg void OnGetArcFileSizeEx();
	afx_msg void OnGetArcOriginalSizeEx();
	afx_msg void OnGetArcCompressedSizeEx();
	afx_msg void OnGetOriginalSizeEx();
	afx_msg void OnGetCompressedSizeEx();
	afx_msg void OnGetCreateTime();
	afx_msg void OnGetAccessTime();
	afx_msg void OnGetArchiveType();
	afx_msg void OnGetCreateTimeEx();
	afx_msg void OnGetAccessTimeEx();
	afx_msg void OnSetUnicodeMode();
	afx_msg void OnSetDefaultPassword();
	afx_msg void OnGetDefaultPassword();
	afx_msg void OnPasswordDialog();
	afx_msg void OnSetPriority();
	afx_msg void OnSfxConfigDialog();
	afx_msg void OnSfxFileStoring();
	afx_msg void OnGetLastError();
	//}}AFX_MSG
	afx_msg LRESULT OnArcExtract(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	HINSTANCE m_hArcDll;
	HARC m_hArc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_7ZTESTDLG_H__74AF8B5B_EAFA_4EEA_9A08_26D0A4ED97E7__INCLUDED_)
