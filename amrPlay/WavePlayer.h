#pragma once
#include "./decoder/WaveDef.h"
#include <vector>
#include <stdafx.h>

class AMRFileDecoder;
class WavePlayer
{
    /// messages
    typedef unsigned int            WaveMsg;     // wave messages
    typedef unsigned int            ThreadMsg;   // thread messages

    enum PlayState
    {
        Play_Stop = 0,
        Play_Pause,
        Play_Playing,
    };

public:
    WavePlayer(void);
    ~WavePlayer(void);

public:
    // ²¥·Å
    void Play(LPCTSTR lpszFile, DWORD* pLength);
    // ÔÝÍ£
    void Pause();
    // ¼ÌÐø
    void Resume();
    // Í£Ö¹
    void Stop();
    // ²¥·ÅÊ±¼ä
    DWORD GetAudioLength(LPCTSTR lpszFile);
    // ÒôÁ¿
    void SetVolume(DWORD dwVolume); // volume: 0-1000

protected:
    static BOOL CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
    BOOL   WaveOutProcImpl(HWAVEOUT hwo, UINT uMsg, DWORD dwParam1, DWORD dwParam2);

    static unsigned int CALLBACK PlayThread( LPVOID lp );                // main play thread
    unsigned int PlayThreadProcImpl();

    static unsigned int CALLBACK DecodeThread(LPVOID lp);
    unsigned int DecodeThreadProcImpl();

    /// prototypes
    WAVEHDR*    allocBlocks( int size, int count );          /// alloc heap for wave header blocks
    void        freeBlocks( WAVEHDR* blockArray );           /// free heap

    void        UnInit();
    void        OnPlayFinished();

private: /// members
    /// thread
    HANDLE                 m_hThreadPlay;
    HANDLE                 m_hThreadDecode;

    PlayState              m_ePlayStat;                        /// ²¥·Å×´Ì¬

    /// Event
    HANDLE                 m_hEventDecode;                     /// event for decoding

    /// device
    HWAVEOUT               m_hWaveoutDev;

    WaveBlock              m_wBlock;
    WaveData               m_waveData;

    ThreadMsg              m_msgPlayThread;                    // thread msg { 1,0 } alive / close
    ThreadMsg              m_msgDecodeThread;

    AMRFileDecoder*        m_pDecoder;

    CRITICAL_SECTION       m_cs;                                       // critical section
};
