#include "AudioDevice.h"

using namespace recgzer_core;

int main()
{
	::CoInitialize(NULL);

	AudioDevice device = AudioDevice::DefaultAudioDevice();


	AudioDeviceId id = device.Id();
	std::wstring name = device.Name();

	return 0;
}