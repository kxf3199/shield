// thread_manager.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "thread_manager.h"

thread_manager* g_pInterface = NULL;

_declspec(dllexport)  thread_manager_interface*  CreateInterface()
{

	if (g_pInterface == NULL)
	{
		return (thread_manager_interface*)new thread_manager(4);
	}
	return (thread_manager_interface*)g_pInterface;
}
_declspec(dllexport) void  ReleaseInterface()
{
	if (g_pInterface)
		delete g_pInterface;
	g_pInterface = NULL;
}



thread_manager::thread_manager(int nSize)
{
	m_threadVector.clear();
	for (int i = 0; i < nSize; i++)
	{
		CThread * tmp = new CThread();
		m_threadVector.push_back(tmp);
	}


}
thread_manager::~thread_manager()
{
	vector<CThread *>::iterator iter = m_threadVector.begin();
	for (; iter != m_threadVector.end();)
	{
		CThread * tmp = *iter++;
		delete tmp;
	}
}
bool thread_manager::pushJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb)
{
	vector<CThread *>::iterator iter = m_threadVector.begin();
	for (; iter != m_threadVector.end(); iter++)
	{
		CThread * tmp = *iter;
		if (!tmp->isWorking())
		{
			tmp->doJob(jobProc, wParam, lParam, cb);
			return true;
		}
	}
	CThread * tmp = new CThread();
	m_threadVector.push_back(tmp);
	tmp->doJob(jobProc, wParam, lParam, cb);
	return true;
}
/************************************************************************/
/* 返回当前在册的装修工的数量                                                                      */
/************************************************************************/
size_t thread_manager::getPoolSize()
{
	return m_threadVector.size();
}
int thread_manager::test()
{
	int nTest = 7;
	return nTest;
}
