#pragma once
#include "HttpClient.h"
#include "WavePlayer.h"

class VoiceManager : public IHttpClientNotify
{
public:
	VoiceManager(void);
	~VoiceManager(void);

	static VoiceManager *getInstance();

	// download
	void downloadFile(CString url, CString filePath, CString fileId, int nTabIndex);

	// stop thread
	void stopHttpThread();

	// play
	void playVoice(CString fileId);

	// stop
	void stopPlay();

protected:
	void OnDownloadCompleted(DWORD dwError, CString fileId, int nTabIndex);

private:
	static VoiceManager *m_pVoiceManager;
	HttpClient *m_pHttpClient;
	WavePlayer   m_player;

};
