#ifndef WEBRTC_VOICE_ENGINE_VOE_MIXER_IMPL_H
#define WEBRTC_VOICE_ENGINE_VOE_MIXER_IMPL_H

#include "voe_mixer.h"

#include "shared_data.h"
#include "webrtc/modules/audio_conference_mixer/source/time_scheduler.h"
#include "webrtc/system_wrappers/interface/scoped_ptr.h"
namespace webrtc
{

class CriticalSectionWrapper;
class VoEMixerImpl: public VoEMixer, Module
{
public:
	// AudioProcessing only accepts 10 ms frames.
    enum {kProcessPeriodicityInMs = 10};
	// Metronome class.
    TimeScheduler _timeScheduler;
	virtual WebRtc_Word32 Process();
	int RegisterExternalStreamReceiver(StreamReceiver & receiver);
	virtual WebRtc_Word32 TimeUntilNextProcess();
	int8_t _playBuffer[3840];
	scoped_ptr<CriticalSectionWrapper> _crit;
	bool init();
	bool start();
	bool stop();

protected:
    VoEMixerImpl(voe::SharedData* shared);
    virtual ~VoEMixerImpl();

    voe::SharedData* _shared;
};

} // namespace webrtc

#endif  // WEBRTC_VOICE_ENGINE_VOE_MIXER_IMPL_H
