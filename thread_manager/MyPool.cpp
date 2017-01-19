#include "stdafx.h"
#include "MyPool.h"

// �ڹ��캯���г�ʼ��ӵ�м��������װ�޹�
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
/* ���ص�ǰ�ڲ��װ�޹�������                                                                      */
/************************************************************************/
int CMyPool::getPoolSize()
{
	return m_threadVector.size();
}
