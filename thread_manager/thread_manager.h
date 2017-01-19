#pragma once
#include "thread.h"
#include "..\shield\interface\thread_manager_interface.h"
#include <vector>
using namespace std;

#ifndef _THREAD_MANAGER_H_
#define _THREAD_MANAGER_H_

#ifdef THREAD_MANAGER_EXPORTS
#define THREAD_MANAGER_API  _declspec(dllexport) 
#else
#define THREAD_MANAGER_API  _declspec(dllimport) 
#endif
THREAD_MANAGER_API thread_manager_interface* CreateInterface();
THREAD_MANAGER_API void ReleaseInterface();
#endif

class THREAD_MANAGER_API thread_manager:public thread_manager_interface
{
public:
	thread_manager(int nSize);
	~thread_manager();
	virtual bool pushJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb);
	virtual size_t getPoolSize();
	virtual int test();
private:
	vector<CThread *> m_threadVector; // 管理所有的thread
	
};
