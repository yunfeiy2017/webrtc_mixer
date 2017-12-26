#pragma once

#include <map>
#include <afxsock.h>
#include <time.h>
#include <map>
#include <process.h>
#include "MixerEngine.h"
#include "voe_base.h"
#include "voe_rtp_rtcp.h"
#include "voe_codec.h"
#include "voe_dtmf.h"
#include "voe_encryption.h"
#include "voe_external_media.h"
#include "voe_file.h"
#include "voe_network.h"
#include "voe_video_sync.h"
#include "voe_volume_control.h"
#include "voe_audio_processing.h"
#include "voe_rtp_rtcp.h"
#include "voe_errors.h"
#include "voe_mixer.h"
#include "MixerUser.h"

using namespace webrtc;

class MediaProcessImpl;
class ConnectionObserver;
class MyEncryption;
class RxCallback;
class MyTransport;

class MixerEngineImpl : public MixerEngine, webrtc::StreamReceiver
{
public:

	MixerEngineImpl(MixerEngineCallback *callback, void *user_data);
	~MixerEngineImpl() ;
	virtual int start() override;
	virtual int stop() override;
	virtual int newPacket(const void *data, int len) override;
	int _id;
	int _numOfChannel;
	VoiceEngine*			_vePtr;
	VoECodec*               _veCodecPtr;
	VoENetwork*             _veNetworkPtr;
	VoEBase*                _veBasePtr;
	VoEMixer*				_veMixerPtr;
	MixerEngineCallback *callback_;
	bool _isStart;
	HANDLE _threadHandle;
	unsigned short m_currPort;
	MixerUser* m_userList[16];// all usersunsigned short MixerConf::GetNextUdpPort()
	int GetNextUdpPort(){
		unsigned short port = m_currPort;
		m_currPort += 2;
		return port;
	}
};

