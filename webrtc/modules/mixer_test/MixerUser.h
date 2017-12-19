#pragma once

//#include <winsock2.h>
#include <afxsock.h>
#include "MixerDefines.h"
#include <stdio.h>
#include <assert.h>
#include "common_types.h"

using namespace webrtc;

class MixerConf;

class MixerUser : public webrtc::Transport
{
public:
	MixerUser(MixerConf& mixerConf, int userID, unsigned short localPort);
	MixerUser(int id, int port, int channel);
	virtual ~MixerUser();
	void PlayData(char *lpRecvData, unsigned int uiRecvLen, sockaddr_in *addrFrom);

	virtual int SendPacket(int channel, const void *data, int len);
	virtual int SendRTCPPacket(int channel, const void *data, int len);
public:
	MixerConf& m_mixerConf;
	int _userID;
	SOCKET _socket;
	unsigned short _localPort;
	sockaddr_in _remoteAddr;
	int _voeChannel;
	bool _isSending;
};

