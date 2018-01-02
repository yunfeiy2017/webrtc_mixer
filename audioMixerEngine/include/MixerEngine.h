#pragma once
#ifdef DLL_IMPLEMENT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#include <map>
namespace mixerengine {
	class DLL_API MixerEngineCallback {
	public:
		MixerEngineCallback() {}
		virtual ~MixerEngineCallback() {}

		virtual void OnMixSamples(const void* audio_data,
			const size_t number_of_frames,
			const size_t bytes_per_sample,
			const size_t number_of_channels,
			const uint32_t sample_rate) = 0;
	};

	class DLL_API MixerEngine
	{
	public:
		static MixerEngine *Create(MixerEngineCallback *callback, void * user_data);
		static void Destroy(MixerEngine *ptr);
		//virtual int newPacket(const void *data, int len) { return 0;}
		virtual int start() = 0;
		virtual int stop() = 0;

	};

}