#pragma once

#include "AudioDeviceId.h"
#include "AudioRecorderType.h"
#include "IAudioRecorder.h"

#include <vector>
#include <memory>

#include <mmdeviceapi.h>
#include <Endpointvolume.h>
#include <Audiopolicy.h>

#include <atlbase.h>

namespace recgzer_core {

	class AudioDevice
	{
	private:
		AudioDevice(IMMDevice* const);
		AudioDevice& operator=(const AudioDevice&);

	public:
		AudioDevice();
		AudioDevice(const AudioDevice& other);
		~AudioDevice();

	public:
		static AudioDevice DefaultAudioDevice();
		static AudioDevice GetAudioDeviceById(const AudioDeviceId&);
		static std::vector<AudioDevice> AudioDevices();

	public:
		AudioDeviceId Id() const;
		std::wstring Name() const;
		float PeakAudioLevel() const;
		std::unique_ptr<IAudioRecorder> Recorder(AudioRecorderType type);

	private:
		CComPtr<IMMDevice> audioDevice;
		CComPtr<IAudioMeterInformation> audioMeterInformation;
		CComPtr<IAudioSessionManager2> audioSessionManager;
	};
}