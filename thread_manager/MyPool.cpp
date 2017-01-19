#include "stdafx.h"
#include "MyPool.h"

// 在构造函数中初始化拥有几个管理的装修工
CMyPool::CMyPool(int nSize)
{
	m_threadVector.clear();
	for (int i = 0; i < nSize;i++)
	{
		CMyThread * tmp = new CMyThread();
		m_threadVector.push_back(tmp);
	}
}


CMyPool::~CMyPool()
{
	vector<CMyThread *>::iterator iter = m_threadVector.begin();
	for (; iter != m_threadVector.end();)
	{
		CMyThread * tmp = *iter++;
		delete tmp;
	}
}

bool CMyPool::pushJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb)
{
	vector<CMyThread *>::iterator iter = m_threadVector.begin();
	for (; iter != m_threadVector.end();iter++)
	{
		CMyThread * tmp = *iter;
		if (!tmp->isWorking())
		{
			tmp->doJob(jobProc, wParam, lParam, cb);
			return true;
		}
	}
	CMyThread * tmp = new CMyThread();
	m_threadVector.push_back(tmp);
	tmp->doJob(jobProc, wParam, lParam, cb);
	return true;
}
/************************************************************************/
/* 返回当前在册的装修工的数量                                                                      */
/************************************************************************/
int CMyPool::getPoolSize()
{
	return m_threadVector.size();
}
