
// shield.h : shield Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "interface\thread_manager_interface.h"

// CshieldApp:
// �йش����ʵ�֣������ shield.cpp
//

class CshieldApp : public CWinAppEx
{
public:
	CshieldApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CRITICAL_SECTION g_cs;
// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	thread_manager_interface* m_pThreadManagerInterface;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CshieldApp theApp;
