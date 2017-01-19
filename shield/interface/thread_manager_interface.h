#pragma once
#include <windows.h>
#include <vector>
using namespace std;
// �߳�Ҫ�����¼�
typedef unsigned int(*jobFunction)(WPARAM wParam, LPARAM lParam);
// �߳���ɺ��֪ͨ�ص�����
typedef void(*jobCallback)(unsigned int pResult);

class _declspec(dllexport) thread_manager_interface
{
public:
	
	virtual ~thread_manager_interface() {};
	virtual bool pushJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb)=0;
	virtual size_t getPoolSize()=0;
	virtual int test() = 0;

};