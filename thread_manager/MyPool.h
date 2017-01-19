#pragma once
#include "MyThread.h"
#include <vector>
using namespace std;
// �̳߳ع����࣬��Ҫȥ��Ϊһ������ͷ������N���߳�,����ͷ
class CMyPool
{
public:
	CMyPool(int nSize);
	~CMyPool();
	bool pushJob(jobFunction jobProc, WPARAM wParam, LPARAM lParam, jobCallback cb);
	int getPoolSize();
private:
	vector<CMyThread *> m_threadVector; // �������е�װ�޹�
};

