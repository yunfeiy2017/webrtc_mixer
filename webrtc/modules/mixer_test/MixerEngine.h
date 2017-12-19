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

using namespace webrtc;

class MediaProcessImpl;
class ConnectionObserver;
class MyEncryption;
class RxCallback;
class MyTransport;


class MixerEngine :public webrtc::StreamReceiver
{
public:
	MixerEngine(int id, int numOfChannel);
	~MixerEngine();
	virtual int newPacket(const void *data, int len) { return 0;}

public:
	int _id;
	int _numOfChannel;
	VoiceEngine*			_vePtr;
	VoECodec*               _veCodecPtr;
	VoENetwork*             _veNetworkPtr;
	VoEBase*                _veBasePtr;
	VoEMixer*				_veMixerPtr;
};

