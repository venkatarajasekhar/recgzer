#include "AudioDevice.h"

using namespace recgzer_core;

int main()
{
	::CoInitialize(NULL);

	AudioDevice device = AudioDevice::DefaultAudioDevice();

	AudioDeviceId id = device.Id();
	std::wstring name = device.DisplayName();

	std::unique_ptr<IAudioRecorder> recorder = device.Recorder(AudioRecorderType::Wave);



	return 0;
}