// 7zTest.h : 7ZTEST アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_7ZTEST_H__7302F58A_D9A0_4879_927C_69F9D424FBF0__INCLUDED_)
#define AFX_7ZTEST_H__7302F58A_D9A0_4879_927C_69F9D424FBF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル
#include "../7-zip32.h"

/////////////////////////////////////////////////////////////////////////////
// CMy7zTestApp:
// このクラスの動作の定義に関しては 7zTest.cpp ファイルを参照してください。
//

class CMy7zTestApp : public CWinApp
{
public:
	CMy7zTestApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMy7zTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CMy7zTestApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_7ZTEST_H__7302F58A_D9A0_4879_927C_69F9D424FBF0__INCLUDED_)
