#pragma once
#include "libHttp.h"
#include "Thread.h"
#include <windows.h>

class IHttpClientNotify
{
public:
	// 文件下载成功
	virtual void OnDownloadCompleted(DWORD dwError, CString fileId, int nTabIndex)=0;
	// 验证连接的有效性，阻止非法连接
// 	virtual bool OnTcpClientNotifyValidate(unsigned long ulType,const void* pData,int nLen)=0;
	// 连接异常
// 	virtual bool OnTcpClientNotifyError(unsigned long ulType,std::string pData,int nLen){return true;};
	// 收到数据包
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

	// 下载文件
	void DownloadFile(CString url, CString filePath, CString fileId, int nTabIndex);

	// 退出
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
