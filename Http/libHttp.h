#ifndef __LibHttp_H__
#define __LibHttp_H__
/**
 * Copyright(c) 2011-2012,  FocusTech. Co., Ltd. 
 * All Rights Reserved.
 *
 * HTTPͨ�š����ؽӿ�
 * Filename: libHttp.h
 *
 *
 * @author  ������
 * @version  1.00    Date: 2012-04-18  2012�°汾
 */
#include <atlstr.h>
#include <string>
#include <map>
#include <vector>
#include "TMErrorCode.h"

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LIBHTTP_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LIBHTTP_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef LIBHTTP_EXPORTS
#define LIBHTTP_API __declspec(dllexport)
#else
#define LIBHTTP_API __declspec(dllimport)
#endif

using std::wstring;
using std::map;

typedef int int32;
typedef unsigned int uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

enum Proxy_Type_e{
	Proxy_Type_NO = 0,
	Proxy_Type_HttpSetting,
	Proxy_Type_Browser,
};

enum ProcessState{
	ProcessState_Idle,				// ����
	ProcessState_StartConnectToServer,
	ProcessState_OpenRequest,
	ProcessState_EndRequest,
	ProcessState_EndRequestDone,
	ProcessState_BeginReceiveData,	// ��ʼ��ȡ����
	ProcessState_ReceiveData,
	ProcessState_ReceivedData,		// �����Ѿ�����, �ȴ����� 
	ProcessState_PartFinished,		// �������
	ProcessState_Finished,
	ProcessState_UserCanceled,
	ProcessState_Failed,
};

enum DLERRCode{
	DLERR_SUCCESS = 0,
	DLERR_INTERNAL = TMERROR_HTTPDOWNLOAD_INTERNAL,				//internal stat 
	DLLER_SERVER_FILENOTFOUND = TMERROR_HTTPDOWNLOAD_FILENOTFOUND,	//���������ļ�δ�ҵ�
	DLERR_CREATEFILE = TMERROR_HTTPDOWNLOAD_CREATEFILE,			//�����ļ�ʧ��
	DLERR_WRITEFILE = TMERROR_HTTPDOWNLOAD_WRITEFILE,			//д���ļ�ʧ��
	DLLER_NETWORK = TMERROR_HTTPDOWNLOAD_NETWORK,				//�������
	DLLER_INTERNAL_WAIT_OBJ = TMERROR_HTTPDOWNLOAD_INTERNAL_WAIT_OBJ,	//�ڲ�����ȴ���������
	DLLER_INTERNAL_CHECKMD5 = TMERROR_HTTPDOWNLOAD_INTERNAL_CHECKMD5,	//�ڲ����� ����md5
	DLLER_USER_CANCEL = TMERROR_HTTPDOWNLOAD_USER_CANCEL,			//�û�ȡ��
	DLLER_SEGENT_ERROR = TMERROR_HTTPDOWNLOAD_SEGENT_ERROR,			//�ﵽ����Դ�������Ȼ�п����س���
};

enum HTTPERROR_e
{
	HTTPERROR_SUCCESS = 0,
	HTTPERROR_INIT,				//��ʼ������
	HTTPERROR_URL,				//URL����
	HTTPERROR_INTERNAL,			//���ڴ���
	HTTPERROR_CONNECT,			//����ʧ��
	HTTPERROR_Send,			    //��������ʧ��
};

enum HttpVerb_e
{
	HTTPVerb_GET = 0,
	HTTPVerb_POST=1,
};

enum HttpHead_e
{
	HTTPHeader_Pragma = 0,
	HTTPHeader_Accept,
	HTTPHeader_CacheControl,
	HTTPHeader_Referer,
	HTTPHeader_UserAgent,
	HTTPHeader_Range,
	HTTPHeader_AcceptCharset,
	HTTPHeader_AcceptEncoding,
	HTTPHeader_AcceptLanguage,
	HTTPHeader_ContentEncoding,
	HTTPHeader_ContentLanguage,
	HTTPHeader_ContentLength,
	HTTPHeader_ContentType,
	HTTPHeader_ContentRange,
};

enum SendRequestState_e{
	SendRequestState_Idle = 0,
	SendRequestState_Connect,
	SendRequestState_Send,
	SendRequestState_Finished,
	SendRequestState_Failed,
};


enum SendResult_e{
    RESULT_FAILED = 0,  // Failed to communicate with the server; try later.
    RESULT_REJECTED,    // Successfully sent the crash report, but the
    // server rejected it; don't resend this report.
    RESULT_SUCCEEDED,   // The server accepted the crash report.
    RESULT_THROTTLED    // No attempt was made to send the crash report, because
    // we exceeded the maximum reports per day.
};

enum UploadState{
	UploadState_Idle = 0,				// ����
	UploadState_BeginUpload,		//��ʼ�ϴ�
	UploadState_Uploaded,			//�ϴ�����
	UploadState_Finished,			//�ϴ�ȫ�����
	UploadStat_UserCanceled,		//�û�ȡ��
	UploadStat_Failed,			//�ϴ�ʧ��
};


struct RemoteFileInfo{
	DWORD nStatusCode;//������״̬
	BOOL bRandomAccess;//������Ӧ�����
	int64 fileSize;//���ص��ļ��ܴ�С 
	int64 fileDownloaded;//�������ļ���С
	
	void Reset()
	{
		nStatusCode = 0;
		bRandomAccess = FALSE;
		fileSize = fileDownloaded = 0;
	}
	void Reset(INT64 _fileSize, INT64 _fileDownloaded, BOOL _bRandomAccess)
	{
		fileSize = _fileSize;
		fileDownloaded = _fileDownloaded;
		bRandomAccess = _bRandomAccess;
	}
};

/**
 * Copyright(c) 2011-2012,  FocusTech. Co., Ltd. 
 * All Rights Reserved.
 *
 * ��ȡ����״̬.
 * Class name : IDownStat
 */
class IDownStat 
{
public:
	/**
      * ��ȡ�������ļ���С
	  * @param void
      * @return �������ļ���С,��λ��B���ֽڣ�
      */
	virtual int64 Downloaded() = 0;

	/**
      * ��ȡ��ǰ�����ٶ�
	  * @param void
      * @return ��ǰ�����ٶȣ�B/s���ֽ�/�룩
      */
	virtual int64 Speed() = 0; 

	/**
      * ��ȡƽ�������ٶ�
	  * @param void
      * @return ƽ�������ٶȣ�B/s���ֽ�/�룩
      */
	virtual int64 AverageSpeed() = 0;

	/**
      * ��ȡ����ʱ��
	  * @param void
      * @return ����ʱ�䣺ms�����룩
      */
	virtual DWORD TimeUsed() = 0;
};


class IHttpAsyncObserver;
class IDownload;
class IFileSender;
class IHttpUploadObserver;
class BreakPointUpload;

/**
 * Copyright(c) 2011-2012,  FocusTech. Co., Ltd. 
 * All Rights Reserved.
 *
 * �ϴ��ļ��۲�����.
 * Class name : IHttpUploadObserver
 */
class IHttpUploadObserver 
{
public:
	/**
      * �۲����¼�����
	  * @param [out]pDownload ���ص�IDownload����ָ��
	  * @param [out]ProcessState ���ص�ProcessState�ṹ��
	  * @param [out]lParam ���صĲ���
      * @return �Ƿ�ɹ�
      */
	virtual BOOL OnHttpUploadEvent( BreakPointUpload* pUpload, UploadState state) = 0;
};

/**
 * Copyright(c) 2011-2012,  FocusTech. Co., Ltd. 
 * All Rights Reserved.
 *
 * �����ļ��й۲�����.
 * Class name : IHttpAsyncObserver
 */
class IHttpAsyncObserver 
{
public:
	/**
      * �۲����¼�����
	  * @param [out]pDownload ���ص�IDownload����ָ��
	  * @param [out]ProcessState ���ص�ProcessState�ṹ��
	  * @param [out]lParam ���صĲ���
      * @return �Ƿ�ɹ�
      */
	virtual BOOL OnHttpAsyncEvent( IDownload* pDownload, ProcessState state, LPARAM lParam ) = 0;
};

/**
 * Copyright(c) 2011-2012,  FocusTech. Co., Ltd. 
 * All Rights Reserved.
 *
 * ʵ��HTTP�ļ����ع���.
 * Class name : IDownload
 */
class IDownload
{
public:
	/**
      * ж��
	  * @param void
      * @return void
      */
	virtual void Release() = 0;

	/**
      * ����һ���۲��ߣ��Թ۲�HTTP����״̬�ı仯��
	  * @param [in]pObserver IHttpAsyncObserver��Ķ���ָ��
      * @return void
      */
	virtual VOID SetObserver( IHttpAsyncObserver *pObserver ) = 0;

	/**
      * �����û���ʶ��
	  * @param [in]lpUserData �û���ʶ��
      * @return �������õ��û���ʶ
      */
	virtual LPVOID SetUserData( LPVOID lpUserData ) = 0;

	/**
      * ����������Ϣ��
	  * @param [in]szUrl Զ�̷�������URL��
	  * @param [in]szFilePath Զ���ļ����ص����ص�ȫ·����
      * @return void
      */
	virtual void SetDownloadInfo( LPCTSTR szUrl, LPCTSTR szFilePath ) = 0;

	/**
      * ���ô����������
	  * @param [in]eProxyType ��ȡ����������ķ�ʽ��
	  * @param [in]szHost �����������ַ��
	  * @param [in]nPort  ����������˿ںš�
      * @return void
      */
	virtual BOOL SetProxy( Proxy_Type_e eProxyType,LPCTSTR szHost = NULL, int nPort = 0 ,CString strLogin = _T(""), CString strPassword = _T("")) = 0;

	/**
      * ��������ͷ�����ݡ�
	  * @param [in]lpHeaders ����ͷ�����ݡ�
      * @return void
      */
	virtual void SetRequestHeaders( LPCTSTR lpHeaders )=0;
	
	/**
      * ��һ�������̣߳�ִ�����ز�����
	  * @param [in]nCorrurent ����ʱ�Ĳ�������
      * @return �Ƿ����ڳɹ�
      */
	virtual BOOL Start( INT nCorrurent = 0 ) = 0;

	/**
      * �ȴ��������
	  * @param void
      * @return void
      */
	virtual void Wait() = 0;

	/**
      * ��һ�������̣߳�ִ�����ز��������ȴ������̵߳Ľ����������󷵻ء�
	  * @param [in]nCorrurent ����ʱ�Ĳ�������
      * @return �Ƿ����ڳɹ�
      */
	virtual BOOL Fetch( INT nCorrurent=0 ) = 0;

	/**
      * ֹͣ���ء�
	  * @param void
      * @return void
      */
	virtual void Stop() = 0;
	
	/**
      * ��ȡ���һ�δ�����롣
	  * @param void
      * @return ���صĴ������
      */
	virtual DLERRCode LastError() = 0;

	/**
      * ��ȡ�û���ʶ��
	  * @param void
      * @return �û���ʶ
      */
	virtual LPVOID GetUserData() = 0;

	/**
      * ��ȡ�������ļ�����Ϣ��
	  * @param void
      * @return RemoteFileInfo�ṹ��ָ�룬�����������ļ�����Ϣ
      */
	virtual RemoteFileInfo *GetRemoteFileInfo() = 0;

	/**
      * ��ȡ����״̬��
	  * @param void
      * @return IDownStat��ָ��
      */
	virtual IDownStat *GetDownloadStat() = 0;

	/**
      * ��ȡ������Ϣ��
	  * @param [out]strUrl �������ط�������URL��
	  * @param [out]strFilePath ���ص����ص�ȫ·����
      * @return void
      */
	virtual void GetDownloadInfo( CString &strUrl, CString &strFilePath ) = 0;
};

class IUploadFile
{
public:
	/**
	  * �����ļ�
	  * @param [in]url �ϴ���ַ
	  * @param [in]parameters ����
	  * @param [in]vecUploadFiles �ϴ��ļ���
	  * @param [out]report_code ����������code
	  * @param [in]strFilePartName �ϴ����ļ����ֵĲ�����
	  * @return void
	  */
	virtual bool SendFile(
		const wstring &url,
		const std::map<wstring, wstring> &parameters,
		const std::vector <wstring> &vecUploadFiles,
		wstring *report_code,
		wstring strFilePartName = L"myfile",
		wstring *report_header = NULL,
		wstring *report_content = NULL
		) = 0;

	virtual void Stop() = 0;

	virtual void Release() = 0;
	/**
	  * ���ó�ʱʱ��
	  * @param [in]dwResolveTimeout �����ʱ
	  * @param [in]dwConnectTimeout ���ӳ�ʱ
	  * @param [in]dwSendTimeout ���ͳ�ʱ
	  * @param [in]dwReceiveTimeout ���ճ�ʱ
	  * @return void
	  */
	virtual void SetTimeout(DWORD dwResolveTimeout, DWORD dwConnectTimeout, DWORD dwSendTimeout, DWORD dwReceiveTimeout) = 0;
};

/**
  * ����һ��HTTP�����ࡣ
  * @param void
  * @return IDownloadָ��
  */
EXTERN_C LIBHTTP_API IDownload* CreateDownloadObject();

/**
  * ����һ��FileSender�ࡣ
  * @param void
  * @return FileSenderָ��
  */
EXTERN_C LIBHTTP_API IUploadFile* CreateUploadObject();

#endif