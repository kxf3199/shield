#pragma once
#include <windows.h>
// װ�޹����߳��࣬ÿ��������ʵ����һ���߳�
// 1.�����̺߳���������ɺ��˳�����
// 2,�߳�������ɺ�ռ�õ���ԴҪ��
// 3.�߳���Ķ�����Ҫ�ɵ���ִ�е�����
// 4.�߳��Ѿ��ڸɻ��ʱ�򣬲�Ҫ��ָ�����������

// �߳�Ҫ�����¼�
typedef unsigned int(*jobFunction)(WPARAM wParam, LPARAM lParam);
// �߳���ɺ��֪ͨ�ص�����
typedef void(*jobCallback)(unsigned int pResult);

class CThread
{
public:
	CThread();
	~CThread();
	bool isWorking();// �ǲ������ڸɻ�
	void doJob(jobFunction jobProc,WPARAM wParam,LPARAM lParam,jobCallback cb); // ȥ���Ҹɵ��... 
private:
	bool bIsWorking;
	jobFunction m_jobFunc; // ����ָ��
	jobCallback m_jobCallback; // �߳�ִ����ɺ�ص�����ָ�� 
	WPARAM wParam;
	LPARAM lParam;
	HANDLE m_hThread;
	void jobDone(); // һ�λ������...
	static DWORD WINAPI threadProc(LPARAM lParam);// �������̺߳���
};

