#include "MixerConf.h"

using namespace std;
using namespace webrtc;

unsigned short MixerConf::m_currPort = UDP_START_PORT;

MixerConf::MixerConf(int id): _id(id)
{	
	_vePtr = VoiceEngine::Create();

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

	for (int i=0;i<CONF_USER_COUNT;i++)
	{
		m_userList[i] = new MixerUser(*this, _id+i, GetNextUdpPort());
		printf("conf:%d AddUser: user:%d socket:%d port:%u\r\n",
			_id, 
			m_userList[i]->_userID,
			m_userList[i]->_socket,
			m_userList[i]->_localPort);
	}
}
MixerConf::~MixerConf()
{
	_veMixerPtr->stop();

	for (int i=0;i<CONF_USER_COUNT;i++)
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
FILE* pcmFile = NULL;
int MixerConf::newPacket(const void *data, int len)
{
	if (pcmFile == NULL)
		pcmFile = fopen("D:\\voip\\mix.pcm", "wb");
	if (pcmFile)
	{
		fwrite(data, 1, len, pcmFile);
		fflush(pcmFile);
	}
	return 0;
}
unsigned short MixerConf::GetNextUdpPort()
{
	unsigned short port = m_currPort;
	m_currPort+=2;
	return port;
}


