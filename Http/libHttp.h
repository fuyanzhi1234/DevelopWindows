#ifndef __LibHttp_H__
#define __LibHttp_H__
/**
 * Copyright(c) 2011-2012,  FocusTech. Co., Ltd. 
 * All Rights Reserved.
 *
 * HTTP通信、下载接口
 * Filename: libHttp.h
 *
 *
 * @author  朱晏翔
 * @version  1.00    Date: 2012-04-18  2012新版本
 */
#include <atlstr.h>
#include <string>
#include <map>
#include <vector>
#include "TMErrorCode.h"

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBHTTP_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBHTTP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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
	ProcessState_Idle,				// 空闲
	ProcessState_StartConnectToServer,
	ProcessState_OpenRequest,
	ProcessState_EndRequest,
	ProcessState_EndRequestDone,
	ProcessState_BeginReceiveData,	// 开始获取数据
	ProcessState_ReceiveData,
	ProcessState_ReceivedData,		// 数据已经接受, 等待处理 
	ProcessState_PartFinished,		// 部分完成
	ProcessState_Finished,
	ProcessState_UserCanceled,
	ProcessState_Failed,
};

enum DLERRCode{
	DLERR_SUCCESS = 0,
	DLERR_INTERNAL = TMERROR_HTTPDOWNLOAD_INTERNAL,				//internal stat 
	DLLER_SERVER_FILENOTFOUND = TMERROR_HTTPDOWNLOAD_FILENOTFOUND,	//服务器的文件未找到
	DLERR_CREATEFILE = TMERROR_HTTPDOWNLOAD_CREATEFILE,			//创建文件失败
	DLERR_WRITEFILE = TMERROR_HTTPDOWNLOAD_WRITEFILE,			//写入文件失败
	DLLER_NETWORK = TMERROR_HTTPDOWNLOAD_NETWORK,				//网络错误
	DLLER_INTERNAL_WAIT_OBJ = TMERROR_HTTPDOWNLOAD_INTERNAL_WAIT_OBJ,	//内部错误等待其他对象
	DLLER_INTERNAL_CHECKMD5 = TMERROR_HTTPDOWNLOAD_INTERNAL_CHECKMD5,	//内部错误 计算md5
	DLLER_USER_CANCEL = TMERROR_HTTPDOWNLOAD_USER_CANCEL,			//用户取消
	DLLER_SEGENT_ERROR = TMERROR_HTTPDOWNLOAD_SEGENT_ERROR,			//达到最大尝试次数，仍然有块下载出错
};

enum HTTPERROR_e
{
	HTTPERROR_SUCCESS = 0,
	HTTPERROR_INIT,				//初始化错误
	HTTPERROR_URL,				//URL错误
	HTTPERROR_INTERNAL,			//内在错误
	HTTPERROR_CONNECT,			//连接失败
	HTTPERROR_Send,			    //发送请求失败
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
	UploadState_Idle = 0,				// 空闲
	UploadState_BeginUpload,		//开始上传
	UploadState_Uploaded,			//上传结束
	UploadState_Finished,			//上传全部完成
	UploadStat_UserCanceled,		//用户取消
	UploadStat_Failed,			//上传失败
};


struct RemoteFileInfo{
	DWORD nStatusCode;//服务器状态
	BOOL bRandomAccess;//服务器应答编码
	int64 fileSize;//下载的文件总大小 
	int64 fileDownloaded;//已下载文件大小
	
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
 * 获取下载状态.
 * Class name : IDownStat
 */
class IDownStat 
{
public:
	/**
      * 获取已下载文件大小
	  * @param void
      * @return 已下载文件大小,单位：B（字节）
      */
	virtual int64 Downloaded() = 0;

	/**
      * 获取当前下载速度
	  * @param void
      * @return 当前下载速度：B/s（字节/秒）
      */
	virtual int64 Speed() = 0; 

	/**
      * 获取平均下载速度
	  * @param void
      * @return 平均下载速度：B/s（字节/秒）
      */
	virtual int64 AverageSpeed() = 0;

	/**
      * 获取下载时间
	  * @param void
      * @return 下载时间：ms（毫秒）
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
 * 上传文件观察者类.
 * Class name : IHttpUploadObserver
 */
class IHttpUploadObserver 
{
public:
	/**
      * 观察者事件函数
	  * @param [out]pDownload 返回的IDownload类型指针
	  * @param [out]ProcessState 返回的ProcessState结构体
	  * @param [out]lParam 返回的参数
      * @return 是否成功
      */
	virtual BOOL OnHttpUploadEvent( BreakPointUpload* pUpload, UploadState state) = 0;
};

/**
 * Copyright(c) 2011-2012,  FocusTech. Co., Ltd. 
 * All Rights Reserved.
 *
 * 下载文件中观察者类.
 * Class name : IHttpAsyncObserver
 */
class IHttpAsyncObserver 
{
public:
	/**
      * 观察者事件函数
	  * @param [out]pDownload 返回的IDownload类型指针
	  * @param [out]ProcessState 返回的ProcessState结构体
	  * @param [out]lParam 返回的参数
      * @return 是否成功
      */
	virtual BOOL OnHttpAsyncEvent( IDownload* pDownload, ProcessState state, LPARAM lParam ) = 0;
};

/**
 * Copyright(c) 2011-2012,  FocusTech. Co., Ltd. 
 * All Rights Reserved.
 *
 * 实现HTTP文件下载功能.
 * Class name : IDownload
 */
class IDownload
{
public:
	/**
      * 卸载
	  * @param void
      * @return void
      */
	virtual void Release() = 0;

	/**
      * 设置一个观察者，以观察HTTP连接状态的变化。
	  * @param [in]pObserver IHttpAsyncObserver类的对象指针
      * @return void
      */
	virtual VOID SetObserver( IHttpAsyncObserver *pObserver ) = 0;

	/**
      * 设置用户标识。
	  * @param [in]lpUserData 用户标识。
      * @return 返回设置的用户标识
      */
	virtual LPVOID SetUserData( LPVOID lpUserData ) = 0;

	/**
      * 设置下载信息。
	  * @param [in]szUrl 远程服务器的URL。
	  * @param [in]szFilePath 远程文件下载到本地的全路径。
      * @return void
      */
	virtual void SetDownloadInfo( LPCTSTR szUrl, LPCTSTR szFilePath ) = 0;

	/**
      * 设置代理服务器。
	  * @param [in]eProxyType 获取代理服务器的方式。
	  * @param [in]szHost 代理服务器地址。
	  * @param [in]nPort  代理服务器端口号。
      * @return void
      */
	virtual BOOL SetProxy( Proxy_Type_e eProxyType,LPCTSTR szHost = NULL, int nPort = 0 ,CString strLogin = _T(""), CString strPassword = _T("")) = 0;

	/**
      * 设置请求头部内容。
	  * @param [in]lpHeaders 请求头部内容。
      * @return void
      */
	virtual void SetRequestHeaders( LPCTSTR lpHeaders )=0;
	
	/**
      * 开一个下载线程，执行下载操作。
	  * @param [in]nCorrurent 下载时的并发数。
      * @return 是否现在成功
      */
	virtual BOOL Start( INT nCorrurent = 0 ) = 0;

	/**
      * 等待下载完成
	  * @param void
      * @return void
      */
	virtual void Wait() = 0;

	/**
      * 开一个下载线程，执行下载操作，并等待下载线程的结束，结束后返回。
	  * @param [in]nCorrurent 下载时的并发数。
      * @return 是否现在成功
      */
	virtual BOOL Fetch( INT nCorrurent=0 ) = 0;

	/**
      * 停止下载。
	  * @param void
      * @return void
      */
	virtual void Stop() = 0;
	
	/**
      * 获取最后一次错误代码。
	  * @param void
      * @return 返回的错误代码
      */
	virtual DLERRCode LastError() = 0;

	/**
      * 获取用户标识。
	  * @param void
      * @return 用户标识
      */
	virtual LPVOID GetUserData() = 0;

	/**
      * 获取服务器文件的信息。
	  * @param void
      * @return RemoteFileInfo结构体指针，包含服务器文件的信息
      */
	virtual RemoteFileInfo *GetRemoteFileInfo() = 0;

	/**
      * 获取下载状态。
	  * @param void
      * @return IDownStat类指针
      */
	virtual IDownStat *GetDownloadStat() = 0;

	/**
      * 获取下载信息。
	  * @param [out]strUrl 返回下载服务器的URL。
	  * @param [out]strFilePath 返回到本地的全路径。
      * @return void
      */
	virtual void GetDownloadInfo( CString &strUrl, CString &strFilePath ) = 0;
};

class IUploadFile
{
public:
	/**
	  * 发送文件
	  * @param [in]url 上传地址
	  * @param [in]parameters 参数
	  * @param [in]vecUploadFiles 上传文件名
	  * @param [out]report_code 服务器返回code
	  * @param [in]strFilePartName 上传的文件部分的参数名
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
	  * 设置超时时间
	  * @param [in]dwResolveTimeout 解决超时
	  * @param [in]dwConnectTimeout 连接超时
	  * @param [in]dwSendTimeout 发送超时
	  * @param [in]dwReceiveTimeout 接收超时
	  * @return void
	  */
	virtual void SetTimeout(DWORD dwResolveTimeout, DWORD dwConnectTimeout, DWORD dwSendTimeout, DWORD dwReceiveTimeout) = 0;
};

/**
  * 创建一个HTTP下载类。
  * @param void
  * @return IDownload指针
  */
EXTERN_C LIBHTTP_API IDownload* CreateDownloadObject();

/**
  * 创建一个FileSender类。
  * @param void
  * @return FileSender指针
  */
EXTERN_C LIBHTTP_API IUploadFile* CreateUploadObject();

#endif