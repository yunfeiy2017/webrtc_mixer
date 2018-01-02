#include "MixerUser.h"
#include "MixerEngine_impl.h"


namespace mixerengine {
	MixerUser::MixerUser(MixerEngineImpl& mixerConf, int userID, unsigned short localPort) : m_mixerConf(mixerConf), dumpPcm(NULL)
	{
		_userID = userID;
		_socket = INVALID_SOCKET;
		_localPort = localPort;
		_voeChannel = -1;
		_isSending = false;

		sockaddr_in addr;
		::memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("172.29.135.1");//INADDR_ANY;//inet_addr("192.168.40.208");
		addr.sin_port = ::htons(_localPort);
		_socket = ::WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (_socket == INVALID_SOCKET)
		{
			printf("conf:%u,user:%u,port:%u create socket failed\r\n", m_mixerConf._id, _userID, _localPort);
		}
		int iRet = ::bind(_socket, (const sockaddr *)&addr, sizeof(addr)); 

		_voeChannel = m_mixerConf._veBasePtr->CreateChannel();
		if (_voeChannel >= 0)
		{
			m_mixerConf._veNetworkPtr->RegisterExternalTransport(_voeChannel, *this);
			m_mixerConf._veBasePtr->StartReceive(_voeChannel);
			m_mixerConf._veBasePtr->StartPlayout(_voeChannel);

			int numCodecs = m_mixerConf._veCodecPtr->NumOfCodecs();
			int idx = 0;
			for (idx = 0; idx < numCodecs; idx++)
			{
				CodecInst codec;
				m_mixerConf._veCodecPtr->GetCodec(idx, codec);
				if ((_stricmp(codec.plname, "opus") == 0))
				{
					m_mixerConf._veCodecPtr->SetSendCodec(_voeChannel, codec);
					break;
				}
			}
			if (idx >= numCodecs)
			{
				CodecInst codec;
				m_mixerConf._veCodecPtr->GetCodec(0, codec);
				m_mixerConf._veCodecPtr->SetSendCodec(_voeChannel, codec);
			}
			m_mixerConf._veBasePtr->StartSend(_voeChannel);
		}
		else
		{
			printf("conf:%u,user:%u create channel failed\r\n", m_mixerConf._id, _userID);
		}
		char file[256];
		sprintf(file, "D:\\voip\\user%d.pcm", _userID);
		dumpPcm = fopen(file, "wb");
		//	_sendBuf = (char*)malloc(SEND_BUF_LEN);
	}

	MixerUser::~MixerUser()
	{
		if (_socket != INVALID_SOCKET)
		{
			closesocket(_socket);
		}
		if (_voeChannel >= 0)
		{
			m_mixerConf._veBasePtr->StopSend(_voeChannel);
			m_mixerConf._veBasePtr->StopReceive(_voeChannel);
			m_mixerConf._veBasePtr->StopPlayout(_voeChannel);
			m_mixerConf._veNetworkPtr->DeRegisterExternalTransport(_voeChannel);
			m_mixerConf._veBasePtr->DeleteChannel(_voeChannel);
		}
		if (dumpPcm)
			fclose(dumpPcm);
		dumpPcm = NULL;
		//	if (_sendBuf)
		{
			//		free(_sendBuf);
		}
	}

	void MixerUser::PlayData(char *lpRecvData, unsigned int uiRecvLen, sockaddr_in *addrFrom)
	{
		if (!_isSending)
		{
			_remoteAddr = *addrFrom;
			_isSending = true;
		}

		CodecInst codec;
		m_mixerConf._veCodecPtr->GetSendCodec(_voeChannel, codec);
		int pltye = lpRecvData[1] & 0x7f;
		if (codec.pltype != pltye)
		{
			m_mixerConf._veBasePtr->StopSend(_voeChannel);
			int numCodecs = m_mixerConf._veCodecPtr->NumOfCodecs();
			int idx = 0;
			for (idx = 0; idx < numCodecs; idx++)
			{
				m_mixerConf._veCodecPtr->GetCodec(idx, codec);
				if (codec.pltype == lpRecvData[1] & 0x7f)
				{
					m_mixerConf._veCodecPtr->SetSendCodec(_voeChannel, codec);
					break;
				}
			}
			if (idx >= numCodecs)
			{
				CodecInst codec;
				m_mixerConf._veCodecPtr->GetCodec(0, codec);
				m_mixerConf._veCodecPtr->SetSendCodec(_voeChannel, codec);
			}

			m_mixerConf._veBasePtr->StartSend(_voeChannel);
		}
		if (dumpPcm)
		{
			fwrite(lpRecvData + 12, 1, uiRecvLen - 12, dumpPcm);
			fflush(dumpPcm);
		}
		m_mixerConf._veNetworkPtr->ReceivedRTPPacket(_voeChannel, lpRecvData, uiRecvLen);
	}

	int MixerUser::SendPacket(int channel, const void *data, int len)
	{
		if (!_isSending)
			return len;
		if (_socket != INVALID_SOCKET)
		{
			DWORD dwSent = 0;
			WSABUF wsaBuffer;
			wsaBuffer.buf = (CHAR*)data;
			wsaBuffer.len = len;
			//todo... 应放到SendBuffer中
			::WSASendTo(_socket, &wsaBuffer, 1, &dwSent, 0, (const sockaddr *)&_remoteAddr, sizeof(_remoteAddr), NULL, NULL);
		}
		return len;
	}
	int MixerUser::SendRTCPPacket(int channel, const void *data, int len)
	{
		return len;
	}

}