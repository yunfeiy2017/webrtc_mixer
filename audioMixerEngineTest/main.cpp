#include "MixerEngine.h"
#include <time.h>
class MyCallback : public MixerEngineCallback
{
public:
	MyCallback() {}
	~MyCallback(){}
	virtual void OnMixSamples()
	{}
};
int main()
{
	MyCallback* callback = new MyCallback();
	MixerEngine* engine = MixerEngine::Create(callback, nullptr);
	engine->start();
	printf("Press ENTER to exit...\r\n");
	getchar();
	engine->stop();
	MixerEngine::Destroy(engine);
	return 0;
}