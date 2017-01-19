#pragma once
#include "MyThread.h"
#include <vector>
using namespace std;
// 线程池管理类，需要去作为一个包工头，管理N个线程,包工头
class CMyPool
{
public:
	CMyPool(int nSize);
	~CMyPool();
	bool pushJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb);
	int getPoolSize();
private:
	vector<CMyThread *> m_threadVector; // 管理所有的装修工
};

