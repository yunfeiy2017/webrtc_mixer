#ifndef WEBRTC_MODULES_MIXER_TEST_MIXER_UDP_RECV_H_
#define WEBRTC_MODULES_MIXER_TEST_MIXER_UDP_RECV_H_
#include "MixerConf.h"
#include "critical_section_wrapper.h"
namespace webrtc {
class CriticalSectionWrapper;
class MixerModule : public Module
{
public:
	// AudioProcessing only accepts 10 ms frames.
    enum {kProcessPeriodicityInMs = 10};
	MixerModule():_timeScheduler(kProcessPeriodicityInMs), _crit(NULL){};
	// Metronome class.
    TimeScheduler _timeScheduler;
	virtual WebRtc_Word32 Process();
	virtual WebRtc_Word32 TimeUntilNextProcess();
	VoEBase* m_voe;
	void setVoe(VoEBase* voe){ m_voe = voe;}
	int8_t                          _playBuffer[3840];
	scoped_ptr<CriticalSectionWrapper> _crit;
	bool init();
};

}
#endif