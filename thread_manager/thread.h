#pragma once
#include <windows.h>
// 装修工，线程类，每个对象中实例化一个线程
// 1.不能线程函数运行完成后退出。。
// 2,线程运行完成后占用的资源要少
// 3.线程类的对象需要可调整执行的任务
// 4.线程已经在干活的时候，不要再指任务给它。。

// 线程要做的事件
typedef unsigned int(*jobFunction)(WPARAM wParam, LPARAM lParam);
// 线程完成后的通知回调函数
typedef void(*jobCallback)(unsigned int pResult);

class CThread
{
public:
	CThread();
	~CThread();
	bool isWorking();// 是不是正在干活
	void doJob(jobFunction jobProc,WPARAM wParam,LPARAM lParam,jobCallback cb); // 去帮我干点活... 
private:
	bool bIsWorking;
	jobFunction m_jobFunc; // 函数指针
	jobCallback m_jobCallback; // 线程执行完成后回调函数指针 
	WPARAM wParam;
	LPARAM lParam;
	HANDLE m_hThread;
	void jobDone(); // 一次活干完了...
	static DWORD WINAPI threadProc(LPARAM lParam);// 真正的线程函数
};

