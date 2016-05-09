#include "VoiceManager.h"
#include "cefclient.h"
#include "XGlobal.h"

VoiceManager *VoiceManager::m_pVoiceManager = NULL;
VoiceManager *VoiceManager::getInstance()
{
	if (m_pVoiceManager == NULL)
	{
		m_pVoiceManager = new VoiceManager;
	}

	return m_pVoiceManager;
}

VoiceManager::VoiceManager(void)
{
	m_pHttpClient = new HttpClient(this);
	m_pHttpClient->StartThread();
}

VoiceManager::~VoiceManager(void)
{
}

void VoiceManager::OnDownloadCompleted(DWORD dwError, CString fileId, int nTabIndex)
{
	// 调用主浏览器的js方法
	CefRefPtr<CefBrowser> browser = XGlobal::inst().TabHost.GetBrowserByIndex(nTabIndex);
	if (browser) 
	{
		CString voiceFileSavePath;
		voiceFileSavePath.Format(_T("%s%s.amr"), GetVoiceCache(), fileId);
		int timeLen = m_player.GetAudioLength(voiceFileSavePath);

		CString wstrTimeLen;
		wstrTimeLen.Format(_T("%d"), timeLen);

		std::string script = "";
		if (dwError == TMERROR_SUCCESS)
		{
			script =  "callbackDownloadVoice('" + WCS2UTF8(fileId) + "', true, '" + WCS2UTF8(wstrTimeLen) + "');";
		}
		else
		{
			script =  "callbackDownloadVoice('" + WCS2UTF8(fileId) + "', false, '" + WCS2UTF8(wstrTimeLen) + "');";
		}

		CefString strJavascript(script);

		// 调用主浏览器的js方法
		browser->GetMainFrame()->ExecuteJavaScript(strJavascript, browser->GetMainFrame()->GetURL(), 0);
	}
}

// download
void VoiceManager::downloadFile(CString url, CString filePath, CString fileId, int nTabIndex)
{
	if (PathFileExists(filePath))
	{
		OnDownloadCompleted(TMERROR_SUCCESS, fileId, nTabIndex);
	}
	else
	{
		m_pHttpClient->DownloadFile(url, filePath, fileId, nTabIndex);
	}
}

void VoiceManager::playVoice(CString fileId)
{
	CString voiceFileSavePath;
	voiceFileSavePath.Format(_T("%s%s.amr"), GetVoiceCache(), fileId);
	// 判断文件是否存在
	if (PathFileExists(voiceFileSavePath)) //不存在，则第一次
	{
		DWORD nAudioLen;        // 播放时间
 		m_player.Play(voiceFileSavePath, &nAudioLen);
	}
}

void VoiceManager::stopPlay()
{
	m_player.Stop();
}

// stop thread
void VoiceManager::stopHttpThread()
{
	m_pHttpClient->Quit();
}

