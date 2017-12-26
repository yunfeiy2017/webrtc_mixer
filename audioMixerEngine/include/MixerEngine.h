#pragma once

#include <map>

class MixerEngineCallback {
public:
	MixerEngineCallback() {}
	virtual ~MixerEngineCallback() {}

	virtual void OnMixSamples() = 0;
};

class MixerEngine
{
public:
	static MixerEngine *Create(MixerEngineCallback *callback, void * user_data);
	static void Destroy(MixerEngine *ptr);
	//virtual int newPacket(const void *data, int len) { return 0;}
	virtual int start() = 0;
	virtual int stop() = 0;

};

