#include "MixerEngine.h"

using namespace std;
using namespace webrtc;

MixerEngine::MixerEngine(int id, int numOfChannel): _id(id), _numOfChannel(numOfChannel)
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

	for (int i=0;i<_numOfChannel;i++)
	{
		
	}
}
MixerEngine::~MixerEngine()
{
	_veMixerPtr->stop();

	for (int i=0;i<_numOfChannel;i++)
	{		
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


