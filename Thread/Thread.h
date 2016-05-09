// AVThread.h: interface for the CAVThreadclass.
//
//////////////////////////////////////////////////////////////////////

#ifndef __THREAD_H__
#define __THREAD_H__

class CThread
{
public:
	CThread();
	virtual ~CThread();
public:
	/*
	�����̣߳��ɹ�����true,ʧ�ܷ���false,ֻ���ڸý��̴������߳�������ϵͳ����ʱ�ŷ���false
	*/
	bool StartThread(void);

    /*
	��ֹ�߳��˳�
	*/
    void StopThread(void);
	/*
	�ȴ��߳��˳������ô˺���ǰ������÷�������ȷ�������̺߳������˳���������ô˺���������
	*/
	void WaitForStop(void);

	/*
	�˺���ֻ�����ڲ�����
	*/
	static DWORD WINAPI InitThreadProc(PVOID pObj){
		return	((CThread*)pObj)->ThreadProc();
	}
	/*
	�˺���ֻ�����ڲ�����
	*/
	unsigned long ThreadProc(void);

	// ��ֹ�߳�
	void Terminate();

protected:
	/*
	���̺߳������������ڴ˺�����ʵ���߳�Ҫ������
	*/
	virtual void ThreadProcMain(void)=0;
protected:
	DWORD	m_dwThreadID;		// �̱߳�ʶ
	HANDLE	m_hThread;			// �߳̾��
	HANDLE	m_evStop;
    bool	m_bWantToStop;              // ��ֹ�߳�
};

#endif
