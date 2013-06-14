#include "WaveAudioRecorder.h"

namespace recgzer_core {

	WaveAudioRecorder::WaveAudioRecorder() {}

	WaveAudioRecorder::WaveAudioRecorder(IMMDevice* device)
	{
		device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&this->audioClient);
	}

	//void WaveAudioRecorder::Start() {}
	//void WaveAudioRecorder::Stop() {}

	int WaveAudioRecorder::Length() { return 0; }
	std::unique_ptr<IAudioStream> WaveAudioRecorder::Stream() { throw; }
}