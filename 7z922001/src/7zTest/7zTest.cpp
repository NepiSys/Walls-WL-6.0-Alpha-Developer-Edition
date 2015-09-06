// 7zTest.cpp : �A�v���P�[�V�����p�N���X�̒�`���s���܂��B
//

#include "stdafx.h"
#include "7zTest.h"
#include "7zTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy7zTestApp

BEGIN_MESSAGE_MAP(CMy7zTestApp, CWinApp)
	//{{AFX_MSG_MAP(CMy7zTestApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy7zTestApp �N���X�̍\�z

CMy7zTestApp::CMy7zTestApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CMy7zTestApp �I�u�W�F�N�g

CMy7zTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMy7zTestApp �N���X�̏�����

BOOL CMy7zTestApp::InitInstance()
{
	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������������
	//  ��Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ��̂��폜����
	//  ���������B

	#ifdef _DEBUG
    CMemoryState oldMemState, newMemState, diffMemState;
    oldMemState.Checkpoint();
	#endif
	{
	CMy7zTestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <��ݾ�> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	}
	#ifdef _DEBUG
    newMemState.Checkpoint();
    if( diffMemState.Difference( oldMemState, newMemState ) )
    {
        TRACE( "Memory leaked!\n" );
    }
	#endif

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
