#pragma once
#include "libHttp.h"
#include "Thread.h"
#include <windows.h>

class IHttpClientNotify
{
public:
	// �ļ����سɹ�
	virtual void OnDownloadCompleted(DWORD dwError, CString fileId, int nTabIndex)=0;
	// ��֤���ӵ���Ч�ԣ���ֹ�Ƿ�����
// 	virtual bool OnTcpClientNotifyValidate(unsigned long ulType,const void* pData,int nLen)=0;
	// �����쳣
// 	virtual bool OnTcpClientNotifyError(unsigned long ulType,std::string pData,int nLen){return true;};
	// �յ����ݰ�
// 	virtual bool OnTcpClientNotifyReceivedFrame(const char* pFrameHeadData,unsigned long nHeadLen, const char* pFrameBodyData,unsigned long nBodyLen)=0;
};

class HttpClient
	: public CThread

{
public:
	HttpClient(IHttpClientNotify *httpClientNotify);
	~HttpClient(void);

private:
	typedef struct _DownloadFileInfo
	{
		CString url;
		CString filePath;
		CString fileId;
		int timeLen;
		int tabIndex;
		_DownloadFileInfo()
		{
			timeLen = 0;
			tabIndex = 1;
		}
	} DownloadFileInfo;

// static HttpClient *getInstance();
public:

	// �����ļ�
	void DownloadFile(CString url, CString filePath, CString fileId, int nTabIndex);

	// �˳�
	void Quit() { m_quit = true; }

private:
	void ThreadProcMain(void);

private:
	IHttpClientNotify *m_pIHttpClientNotify;
	IDownload *m_pIDownload;
	bool m_quit;
	std::vector<DownloadFileInfo> m_downloadFileInfos;
// 	static HttpClient *m_pHttpClient;

};
