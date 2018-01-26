#include "MixerEngine.h"
#include <time.h>
using namespace mixerengine;
class MyCallback : public MixerEngineCallback
{
public:
	MyCallback() {}
	~MyCallback(){}
	virtual void OnMixSamples(const void* audio_data,
		const size_t number_of_frames,
		const size_t bytes_per_sample,
		const size_t number_of_channels,
		const uint32_t sample_rate)
	{}
};
int main()
{
	MyCallback* callback = new MyCallback();
	MixerEngine* engine = MixerEngine::Create(callback, nullptr);
	engine->start();
	printf("Press ENTER to exit...\r\n");
	getchar();
	short* data[80];
	engine->addAECFarendData(data, 80, 1, 8000);
	engine->stop();
	MixerEngine::Destroy(engine);
	return 0;
}