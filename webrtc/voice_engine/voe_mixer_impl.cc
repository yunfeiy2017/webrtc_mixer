#include "voe_mixer_impl.h"

#include "channel.h"
#include "critical_section_wrapper.h"
#include "trace.h"
#include "voe_errors.h"
#include "voice_engine_impl.h"
#include "output_mixer.h"
namespace webrtc
{

VoEMixer* VoEMixer::GetInterface(VoiceEngine* voiceEngine)
{
#ifndef WEBRTC_VOICE_ENGINE_MIXER_API
    return NULL;
#else
    if (NULL == voiceEngine)
    {
        return NULL;
    }
    VoiceEngineImpl* s = reinterpret_cast<VoiceEngineImpl*>(voiceEngine);
    s->AddRef();
    return s;
#endif
}

#ifdef WEBRTC_VOICE_ENGINE_MIXER_API

VoEMixerImpl::VoEMixerImpl(voe::SharedData* shared) :_timeScheduler(kProcessPeriodicityInMs), _crit(NULL),  _shared(shared)
{
    WEBRTC_TRACE(kTraceMemory, kTraceVoice, VoEId(_shared->instance_id(), -1),
                 "VoEMixerImpl() - ctor");
	init();
}

VoEMixerImpl::~VoEMixerImpl()
{
    WEBRTC_TRACE(kTraceMemory, kTraceVoice, VoEId(_shared->instance_id(), -1),
                 "~VoEMixerImpl() - dtor");
}

WebRtc_Word32 VoEMixerImpl::Process()
{
	CriticalSectionScoped cs(_crit.get());

    // Let the scheduler know that we are running one iteration.
    _timeScheduler.UpdateScheduler();
	assert(_shared->output_mixer() != NULL);
	return _shared->output_mixer()->MixActiveChannels();

}

int VoEMixerImpl::RegisterExternalStreamReceiver(StreamReceiver& receiver)
{
	assert(_shared->output_mixer() != NULL);
	return _shared->output_mixer()->RegisterExternalStreamReceiver(receiver);
}

// Process should be called every kProcessPeriodicityInMs ms
WebRtc_Word32 VoEMixerImpl::TimeUntilNextProcess()
{
    WebRtc_Word32 timeUntilNextProcess = 0;
    CriticalSectionScoped cs(_crit.get());
    if(_timeScheduler.TimeToNextUpdate(timeUntilNextProcess) != 0)
    {
        // Sanity check
        assert(false);
        return -1;
    }
    return timeUntilNextProcess;
}

bool VoEMixerImpl::init()
{
	_crit.reset(CriticalSectionWrapper::CreateCriticalSection());
    if (_crit.get() == NULL)
        return false;
	return true;
}

bool VoEMixerImpl::start()
{
	if (_shared->process_thread() &&
        _shared->process_thread()->RegisterModule(this) != 0)
    {
        _shared->SetLastError(VE_AUDIO_CONF_MIX_MODULE_ERROR, kTraceError,
            "start() failed to register the mixer");
        return false;
    }
	return true;
}

bool VoEMixerImpl::stop()
{
	if (_shared->process_thread()->DeRegisterModule(this) != 0)
    {
        _shared->SetLastError(VE_THREAD_ERROR, kTraceError,
            "stop() failed to deregister mixer");
		return false;
    }
	return true;
}


#endif  // WEBRTC_VOICE_ENGINE_MIXER_API



} // namespace webrtc

