// Thread.cpp: implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Thread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThread::CThread()
{
    m_bWantToStop = false;
	m_dwThreadID=0;
	m_hThread=NULL;
	m_evStop = CreateEvent(NULL,TRUE,TRUE,NULL);
	SetEvent(m_evStop);
}

CThread::~CThread()
{
	if(m_evStop)
	{
		CloseHandle(m_evStop);
		m_evStop = NULL;
	}
	m_hThread = NULL;
}

bool CThread::StartThread()
{
	//如果线程已经创建，则不需要再创建
	if (!m_hThread){ 
		m_bWantToStop = false;
		//创建并启动线程函数
		m_hThread = CreateThread(
					NULL,			//the handle cannot be inherited. 
                    0,				//the default Thread Stack Size is set
                    InitThreadProc,	//线程函数
                    this,			//线程函数的参数
                    0,				//使线程函数创建完后立即启动
                    &m_dwThreadID	//receives the thread identifier
					);
                
        }//end if (!m_hThread...

	if (m_hThread)
		ResetEvent(m_evStop);

	return	m_hThread != NULL;
}

// 终止线程
void CThread::StopThread()
{

    m_bWantToStop = true;
    CThread::WaitForStop();
}

void CThread::WaitForStop()
{	
	if (m_evStop)
	{
		WaitForSingleObject(m_evStop,INFINITE);
		CloseHandle(m_evStop);
		m_evStop = NULL;
	}
	

	// 返回线程句柄
	HANDLE hThread = (HANDLE)InterlockedExchange((LONG *)&m_hThread, 0);
	if (hThread) {
		// 等待线程终止
		WaitForSingleObject(hThread, INFINITE);
		// 关闭线程句柄
		CloseHandle(hThread);
	}// end if (hThread...
}

unsigned long  CThread::ThreadProc()
{
	ThreadProcMain();
	SetEvent(m_evStop);
	return 0;
}

// 终止线程
void CThread::Terminate()
{
	TerminateThread(m_hThread, 0);
}