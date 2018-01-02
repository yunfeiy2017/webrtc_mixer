#pragma once
#include <afxsock.h>
#include <map>
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
//class MixerModule;


class MixerConf :public webrtc::StreamReceiver
{
public:
	MixerConf(int id);
	~MixerConf();
	static unsigned short GetNextUdpPort();
	static unsigned short m_currPort;
	virtual int newPacket(const void *data, int len);

public:
	int _id;
	VoiceEngine*			_vePtr;
	VoECodec*               _veCodecPtr;
	VoENetwork*             _veNetworkPtr;
	VoEBase*                _veBasePtr;
	VoEMixer*				_veMixerPtr;

	MixerUser* m_userList[CONF_USER_COUNT];// all users
};

