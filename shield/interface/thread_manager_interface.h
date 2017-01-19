#pragma once
#include <windows.h>
#include <vector>
using namespace std;
// 线程要做的事件
typedef unsigned int(*jobFunction)(WPARAM wParam, LPARAM lParam);
// 线程完成后的通知回调函数
typedef void(*jobCallback)(unsigned int pResult);

class _declspec(dllexport) thread_manager_interface
{
public:
	
	virtual ~thread_manager_interface() {};
	virtual bool pushJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb)=0;
	virtual size_t getPoolSize()=0;
	virtual int test() = 0;

};