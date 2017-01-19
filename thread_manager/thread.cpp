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
// 		TerminateThread(m_hThread, 0); // ǿ������ɱ
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
/* ִ��һ�����������                                                                     */
/************************************************************************/
void CThread::doJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb)
{
	this->m_jobCallback = cb;
	this->m_jobFunc = jobProc;
	this->wParam = wParam;
	this->lParam = lParam;
	ResumeThread(m_hThread); // �ָ��߳�,��װ�޹������ɻ���..
	//printf("thread %08X start work..wParam %d  lParam %d\n", m_hThread, wParam, lParam);
}

void CThread::jobDone()
{
	bIsWorking = false;
	SuspendThread(m_hThread); // �߳�˯��ȥ��...
}

/************************************************************************/
/* �������̺߳���                                                         */
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
		{// ��Ϊ�߳����ڹ��캯���������,���������ʱ��û�о��������..�������ʱ����ȥ�ɻ�...
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
