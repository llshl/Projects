
// Block.h : Block ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CBlockApp:
// �� Ŭ������ ������ ���ؼ��� Block.cpp�� �����Ͻʽÿ�.
//

class CBlockApp : public CWinAppEx
{
public:
	CBlockApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBlockApp theApp;
