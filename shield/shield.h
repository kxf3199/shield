
// shield.h : shield 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "interface\thread_manager_interface.h"

// CshieldApp:
// 有关此类的实现，请参阅 shield.cpp
//

class CshieldApp : public CWinAppEx
{
public:
	CshieldApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CRITICAL_SECTION g_cs;
// 实现
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
