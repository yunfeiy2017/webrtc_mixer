#ifndef WEBRTC_VOICE_ENGINE_VOE_MIXER_H
#define WEBRTC_VOICE_ENGINE_VOE_MIXER_H

#include "common_types.h"

namespace webrtc {

class VoiceEngine;

class WEBRTC_DLLEXPORT VoEMixer
{
public:
    // Factory for the VoEMixer sub-API. Increases an internal
    // reference counter if successful. Returns NULL if the API is not
    // supported or if construction fails.
    static VoEMixer* GetInterface(VoiceEngine* voiceEngine);

	virtual int RegisterExternalStreamReceiver(
		StreamReceiver& transport) = 0;

    // Releases the VoEMixer sub-API and decreases an internal
    // reference counter. Returns the new reference count. This value should
    // be zero for all sub-API:s before the VoiceEngine object can be safely
    // deleted.
    virtual int Release() = 0;

	virtual bool start() = 0;
	virtual bool stop() = 0;

protected:
    VoEMixer() {}
    virtual ~VoEMixer() {}
};

} // namespace webrtc

#endif  //  WEBRTC_VOICE_ENGINE_VOE_MIXER_H

