#include "MixerModule.h"
namespace webrtc {
WebRtc_Word32 MixerModule::Process()
{
	WebRtc_UWord32 nSamplesOut(0);
	CriticalSectionScoped cs(_crit.get());

    // Let the scheduler know that we are running one iteration.
    _timeScheduler.UpdateScheduler();
	return m_voe->NeedMorePlayData(80,
        2,
        1,
        8000,//const WebRtc_UWord32 samplesPerSec,
        _playBuffer,//void* audioSamples,
        nSamplesOut//WebRtc_UWord32& nSamplesOut
		);

}

// Process should be called every kProcessPeriodicityInMs ms
WebRtc_Word32 MixerModule::TimeUntilNextProcess()
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

bool MixerModule::init()
{
	_crit.reset(CriticalSectionWrapper::CreateCriticalSection());
    if (_crit.get() == NULL)
        return false;
}
}