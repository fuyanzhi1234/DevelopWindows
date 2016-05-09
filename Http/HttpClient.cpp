#include "HttpClient.h"

// HttpClient *HttpClient::m_pHttpClient = NULL;
// HttpClient *HttpClient::getInstance()
// {
// 	if (m_pHttpClient == NULL)
// 	{
// 		m_pHttpClient = new HttpClient
// 	}
// }

HttpClient::HttpClient(IHttpClientNotify *httpClientNotify)
: m_pIHttpClientNotify(httpClientNotify)
{
	m_pIDownload = CreateDownloadObject( );
}

HttpClient::~HttpClient(void)
{
}

void HttpClient::ThreadProcMain(void)
{
	while (1)
	{
		if (m_quit == true)
		{
			m_pIDownload->Release();
			m_pIDownload = NULL;
			break;
		}
		if (m_downloadFileInfos.size() > 0)
		{
			DownloadFileInfo fileInfo = m_downloadFileInfos.back();
			m_downloadFileInfos.pop_back();
			DWORD dwError = TMERROR_SUCCESS;
			m_pIDownload->SetDownloadInfo(fileInfo.url, fileInfo.filePath);
			if (m_pIDownload->Fetch(1) == FALSE)
			{
				DWORD nStatusCode = m_pIDownload->GetRemoteFileInfo()->nStatusCode;
				DLERRCode e = m_pIDownload->LastError();
				dwError = (DWORD)e + nStatusCode;
			}
			else
			{
				//判断文件大小
				if (m_pIDownload->GetRemoteFileInfo()->fileSize == 0)
				{
	 				dwError = TMERROR_AUTOUPDATE_DOWNLOADFILE_EMPTY;
				}
			}
			m_pIHttpClientNotify->OnDownloadCompleted(dwError, fileInfo.fileId, fileInfo.tabIndex);
		}
		Sleep(10);
	}
}

// 下载文件
void HttpClient::DownloadFile(CString url, CString filePath, CString fileId, int nTabIndex)
{
	DownloadFileInfo fileInfo;
	fileInfo.url = url;
	fileInfo.filePath = filePath;
	fileInfo.fileId = fileId;
	fileInfo.tabIndex = nTabIndex;
	m_downloadFileInfos.push_back(fileInfo);
}
