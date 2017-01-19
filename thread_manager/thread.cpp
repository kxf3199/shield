#include "stdafx.h"
#include "thread.h"


CThread::CThread()
{
	bIsWorking = false;
	m_jobFunc = 0;
	m_jobCallback = 0;
	wParam = 0;
	lParam = 0;
	m_hThread = CreateThread(0,0,(LPTHREAD_START_ROUTINE)threadProc,this,0,0);
}


CThread::~CThread()
{
// 	if (m_hThread != INVALID_HANDLE_VALUE)
// 	{
// 		TerminateThread(m_hThread, 0); // 强制性自杀
// 	}
	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		m_jobCallback = (jobCallback)INVALID_HANDLE_VALUE;
		m_jobFunc = (jobFunction)INVALID_HANDLE_VALUE;
		ResumeThread(m_hThread);
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

bool CThread::isWorking()
{
	return this->bIsWorking;
}
/************************************************************************/
/* 执行一个具体的任务                                                                     */
/************************************************************************/
void CThread::doJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb)
{
	this->m_jobCallback = cb;
	this->m_jobFunc = jobProc;
	this->wParam = wParam;
	this->lParam = lParam;
	ResumeThread(m_hThread); // 恢复线程,叫装修工起来干活了..
	//printf("thread %08X start work..wParam %d  lParam %d\n", m_hThread, wParam, lParam);
}

void CThread::jobDone()
{
	bIsWorking = false;
	SuspendThread(m_hThread); // 线程睡觉去吧...
}

/************************************************************************/
/* 真正的线程函数                                                         */
/************************************************************************/
DWORD WINAPI CThread::threadProc(LPARAM lParam)
{
	CThread * pThis = (CThread *)lParam;
	while (true)
	{
		if (pThis->m_jobCallback ==INVALID_HANDLE_VALUE || pThis->m_jobFunc == INVALID_HANDLE_VALUE)
		{
			//printf("thread %08X see byebye", pThis->m_hThread);
			break;
		}
		if (pThis->m_jobCallback == 0 || pThis->m_jobFunc == 0)
		{// 因为线程是在构造函数中申请的,但是申请的时候没有具体的任务..所以这个时候不能去干活...
			pThis->jobDone();
		}
		pThis->bIsWorking = true;
		unsigned int result = pThis->m_jobFunc(pThis->wParam, pThis->lParam);
		//printf("thread %08X job result %d", pThis->m_hThread, result);
		pThis->m_jobCallback(result);
		pThis->jobDone();
	}
	return 0;
}
