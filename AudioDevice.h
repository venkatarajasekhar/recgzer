#pragma once

#include <vector>

#include <mmdeviceapi.h>
#include <Endpointvolume.h>
#include <Audiopolicy.h>

#include <atlbase.h>

// TODO: remove the smart pointers to 

namespace YoutubeScrobbler {

	class AudioSession;

	class AudioDevice
	{
	private:
		AudioDevice();		
		AudioDevice(IMMDevice* const);
		AudioDevice& operator=(const AudioDevice&);	
	public:
		AudioDevice(const AudioDevice& other);
		~AudioDevice();
	public:
		static AudioDevice GetDefaultAudioDevice();
		static AudioDevice GetAudioDeviceById(const std::wstring&);
		static std::vector<AudioDevice> GetAudioDevices();	
	public:
		std::wstring GetId();
		std::wstring GetName();
		float GetPeakAudioLevel();
		std::vector<AudioSession> GetAudioSessions();
	private:
		IMMDevice* audioDevice;
		IAudioMeterInformation* audioMeterInformation;
		IAudioSessionManager2* audioSessionManager; 
	};
}