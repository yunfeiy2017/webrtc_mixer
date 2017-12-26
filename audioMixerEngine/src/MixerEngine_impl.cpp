#include "MixerEngine_impl.h"
#include <time.h>
#include <map>
#include <process.h>

using namespace std;
using namespace webrtc;

typedef enum _IO_OPERATION
{
	IO_ACCEPT,
	IO_RECV,
	IO_SEND,
	IO_RECVFROM,
	IO_TCPNOTICE,
} IO_OPERATION;

//单IO数据，扩展的WSAOVERLAPPED
typedef struct _PER_IO_CONTEXT
{
	WSAOVERLAPPED              ol;
	IO_OPERATION               IoOperation;
	SOCKET                     sAccept;
	u_long                     ulSocketCheck;
	u_int					   wParam;
	long					   lParam;
	WSABUF                     wsaBuffer;
} PER_IO_CONTEXT, *LPPER_IO_CONTEXT;

#define GetRecvFromAddr(_pIo) ((struct sockaddr *)(_pIo + sizeof(PER_IO_CONTEXT)))
#define GetRecvFromLen(_pIo) ((int *)(_pIo + sizeof(PER_IO_CONTEXT) + sizeof(sockaddr_in) + 16))

#define PrepareForRecvFrom(_pIo) \
	{ \
		_pIo->wsaBuffer.buf += sizeof(sockaddr_in) + 16 + sizeof(int); \
		_pIo->wsaBuffer.len -= sizeof(sockaddr_in) + 16 + sizeof(int); \
		*GetRecvFromLen(_pIo) = sizeof(sockaddr_in) + 16; \
	}

LPPER_IO_CONTEXT AllocIoContext(u_long buflen)
{
	LPPER_IO_CONTEXT p = (LPPER_IO_CONTEXT)malloc(sizeof(PER_IO_CONTEXT) + buflen);
	memset(&(p->ol), 0x00, sizeof(p->ol));
	p->wsaBuffer.buf = (char*)(p + 1);
	p->wsaBuffer.len = buflen;
	p->sAccept = INVALID_SOCKET;
	p->lParam = 0;
	p->wParam = 0;
	p->ulSocketCheck = 0;
	return p;
}

bool g_run = false;
bool g_end = false;
std::map<SOCKET, MixerUser*> g_mapSocketUser;
std::map<SOCKET, LPPER_IO_CONTEXT> g_mapSocketIoContext;

unsigned __stdcall EntryPoint(void* pArg)
{
	HANDLE m_hIocp;
	int iError;
	SOCKET sockCurr;
	SOCKET sockAccept;
	IO_OPERATION IoOperation;
	DWORD dwNumberBytes, dwCompletionKey;
	bool bOnSend;
	MixerEngineImpl* impl = (MixerEngineImpl*)pArg;

	if ((m_hIocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		printf("CreateIoCompletionPort() failed with error: %d\r\n", ::WSAGetLastError());
		return -1;
	}

	{
		for (int j = 0; j<CONF_USER_COUNT; j++)
		{
			MixerUser* user = impl->m_userList[j];
			SOCKET& s = user->_socket;
			if (s == INVALID_SOCKET)
			{
				continue;
			}

			//将UDP套接字和已经建立的完成端口关联起来
			if (::CreateIoCompletionPort((HANDLE)s, m_hIocp, s, 0) == NULL)
			{
				printf("CreateIoCompletionPort(Udp Socket) failed with error: %d\r\n", GetLastError());
				::closesocket(s);
				s = INVALID_SOCKET;
				continue;
			}

			LPPER_IO_CONTEXT lpIoContext = AllocIoContext(RECV_BUF_LEN);
			lpIoContext->IoOperation = IO_RECVFROM;
			DWORD dwFlags = 0;
			DWORD dwRecvBytes = 0;
			PrepareForRecvFrom(lpIoContext);
			int ret = ::WSARecvFrom(s, &(lpIoContext->wsaBuffer), 1, &dwRecvBytes, &dwFlags,
				GetRecvFromAddr(lpIoContext), GetRecvFromLen(lpIoContext), &(lpIoContext->ol), NULL);

			g_mapSocketUser[s] = user;
			g_mapSocketIoContext[s] = lpIoContext;
		}
	}

	while (impl->_isStart)
	{
		LPPER_IO_CONTEXT lpIoContext;
		dwNumberBytes = 0;
		dwCompletionKey = INVALID_SOCKET;
		lpIoContext = NULL;
		BOOL bRet = ::GetQueuedCompletionStatus(m_hIocp, &dwNumberBytes, &dwCompletionKey, (LPOVERLAPPED*)&lpIoContext, 10);
		if (!bRet)
		{
			if (NULL == lpIoContext) //处理第2种情况  
				continue;

			//pOverlapped !=NULL  
			DWORD dwErr = GetLastError();
			if (dwCompletionKey == NULL)
			{
				continue;
			}
			else
			{
				printf("error code is %d", dwErr); //处理第3和第4种情况  
				continue;
			}
			continue;
		}
		sockCurr = (SOCKET)dwCompletionKey;
		IoOperation = lpIoContext->IoOperation;
		if (IoOperation == IO_RECVFROM)
		{
			sockaddr_in * lpFrom = (sockaddr_in *)GetRecvFromAddr(lpIoContext);
			std::map<SOCKET, MixerUser*>::iterator it = g_mapSocketUser.find(sockCurr);
			if (it == g_mapSocketUser.end())
			{
				return -1;
			}
			MixerUser* user = g_mapSocketUser[sockCurr];
			user->PlayData(lpIoContext->wsaBuffer.buf, dwNumberBytes, lpFrom);

			memset(&(lpIoContext->ol), 0x00, sizeof(lpIoContext->ol));
			DWORD dwFlags = 0;
			DWORD dwRecvBytes = 0;
			int ret = ::WSARecvFrom(sockCurr, &(lpIoContext->wsaBuffer), 1, &dwRecvBytes, &dwFlags,
				GetRecvFromAddr(lpIoContext), GetRecvFromLen(lpIoContext), &(lpIoContext->ol), NULL);
		}
	}
	::PostQueuedCompletionStatus(m_hIocp, 0, INVALID_SOCKET, 0);
	if (m_hIocp)
	{
		::CloseHandle(m_hIocp);
		m_hIocp = NULL;
	}

	for (std::map<SOCKET, LPPER_IO_CONTEXT>::iterator it = g_mapSocketIoContext.begin(); it != g_mapSocketIoContext.end(); it++)
	{

		free(it->second);
	}
	g_mapSocketIoContext.clear();
	g_mapSocketUser.clear();

	g_end = true;

	_CrtDumpMemoryLeaks();

	return 0;
}
MixerEngine * MixerEngine::Create(MixerEngineCallback * callback, void * user_data)
{
	return static_cast<MixerEngine *>(new MixerEngineImpl(callback, user_data));
}
void MixerEngine::Destroy(MixerEngine * ptr)
{
	if (ptr != nullptr) {
		delete ptr;
		ptr = nullptr;
	}
}
MixerEngineImpl::MixerEngineImpl(MixerEngineCallback *callback, void *user_data):callback_(callback), _isStart(false), m_currPort(6666)
{	
	WORD   wVersionRequested;//定义socket1.1或者socket2.0     
	WSADATA   wsaData;   //定义装载socket版本的变量  
	int   err;   //错误变量  

	wVersionRequested = MAKEWORD(2, 2);   //定义连接为socket2.0  

	err = WSAStartup(wVersionRequested, &wsaData);   //装载socket2.0支持  
	if (0 != err)//判断是否装载成功  
	{
		printf("WSAStartup failed");
	}
	_vePtr = VoiceEngine::Create();
	_numOfChannel = 8;
	_id = 0;

    VoiceEngine::SetTraceFilter(kTraceNone);
	VoiceEngine::SetTraceFile("ve_win_test.txt");
    VoiceEngine::SetTraceCallback(NULL);
	
    if (_vePtr)
    {
        _veNetworkPtr = VoENetwork::GetInterface(_vePtr);
        _veBasePtr = VoEBase::GetInterface(_vePtr);
        _veCodecPtr = VoECodec::GetInterface(_vePtr);
		_veMixerPtr = VoEMixer::GetInterface(_vePtr);
    }
	_veBasePtr->Init(); 
	_veMixerPtr->start();
	_veMixerPtr->RegisterExternalStreamReceiver(*this);

	for (int i=0;i<_numOfChannel;i++)
	{
		m_userList[i] = new MixerUser(*this, _id + i, GetNextUdpPort());
		printf("conf:%d AddUser: user:%d socket:%d port:%u\r\n",
			_id,
			m_userList[i]->_userID,
			m_userList[i]->_socket,
			m_userList[i]->_localPort);
	}
}
MixerEngineImpl::~MixerEngineImpl()
{
	if (_isStart)
		stop();
	_veMixerPtr->stop();

	for (int i=0;i<_numOfChannel;i++)
	{		
		delete m_userList[i];
	}

    if (_veCodecPtr) _veCodecPtr->Release();
    if (_veNetworkPtr) _veNetworkPtr->Release();
	if (_veBasePtr) _veBasePtr->Release();
    if (_vePtr)
    {
        VoiceEngine::Delete(_vePtr);
    }
    VoiceEngine::SetTraceFilter(kTraceNone);
}

int MixerEngineImpl::start()
{	
	unsigned int id = 0;
	
	_isStart = true;
	if ((_threadHandle = (HANDLE)_beginthreadex(NULL, 0, EntryPoint, this, CREATE_SUSPENDED, &id)) == NULL)
	{
		printf("create recv thread fail exit\n");
		return -1;
	}

	if (ResumeThread(_threadHandle) == -1)
	{
		printf("resume recv thread fail exit\n");
		return -1;
	}
}
FILE* pcmFile = NULL;
int MixerEngineImpl::stop()
{
	if (_isStart)
		_isStart = false;
	TerminateThread(_threadHandle, 0);
	if (pcmFile)
		fclose(pcmFile);
	return 0;
}

int MixerEngineImpl::newPacket(const void * data, int len)
{
	if (pcmFile == NULL)
		pcmFile = fopen("D:\\voip\\mix.pcm", "wb");
	if (pcmFile)
	{
		fwrite(data, 1, len, pcmFile);
		fflush(pcmFile);
	}
	return 0;
	if (callback_)
		callback_->OnMixSamples();

	return 0;
}


