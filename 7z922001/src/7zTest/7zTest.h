// 7zTest.h : 7ZTEST �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_7ZTEST_H__7302F58A_D9A0_4879_927C_69F9D424FBF0__INCLUDED_)
#define AFX_7ZTEST_H__7302F58A_D9A0_4879_927C_69F9D424FBF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��
#include "../7-zip32.h"

/////////////////////////////////////////////////////////////////////////////
// CMy7zTestApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� 7zTest.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CMy7zTestApp : public CWinApp
{
public:
	CMy7zTestApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMy7zTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CMy7zTestApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_7ZTEST_H__7302F58A_D9A0_4879_927C_69F9D424FBF0__INCLUDED_)
