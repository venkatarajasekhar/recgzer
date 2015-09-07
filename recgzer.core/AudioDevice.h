#pragma once

#include "AudioDeviceId.h"
#include "AudioRecorderType.h"
#include "IAudioRecorder.h"

/**/#include "WaveAudioRecorder.h"

#include <vector>
#include <memory>

#include <mmdeviceapi.h>
#include <Endpointvolume.h>
#include <Audiopolicy.h>

#include <atlbase.h>

namespace recgzer_core {

	class AudioDevice
	{
	public:
		AudioDevice() {} // Empty
		AudioDevice(const AudioDevice& other);
		~AudioDevice() {} // Empty
		static AudioDevice DefaultAudioDevice();
		static AudioDevice GetAudioDeviceById(const AudioDeviceId&);
		static std::vector<AudioDevice> AudioDevices();
		AudioDeviceId Id() const;
		std::wstring Name() const;
		float PeakAudioLevel() const;
		std::unique_ptr<WaveAudioRecorder> Recorder(AudioRecorderType) const;

	private:
	        AudioDevice(IMMDevice* const);
		AudioDevice& operator=(const AudioDevice&); // Not Implemented
		CComPtr<IMMDevice> audioDevice;
		CComPtr<IAudioMeterInformation> audioMeterInformation;
		CComPtr<IAudioSessionManager2> audioSessionManager;
	};
}
