#include "AudioDevice.h"

#include "AudioSession.h"
#include "__HR_AudioDevice.h"

#include <vector>

#include <mmdeviceapi.h>
#include <Endpointvolume.h>
#include <Audiopolicy.h>
#include <Functiondiscoverykeys_devpkey.h>

#include <atlbase.h>

using namespace std;

namespace YoutubeScrobbler {

	AudioDevice::AudioDevice(IMMDevice* const audioDevice)
	{
		this->audioDevice = audioDevice;
		__HR_AudioDevice::ThrowIfNot_SOK(this->audioDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, nullptr, (void**)&this->audioMeterInformation));
		__HR_AudioDevice::ThrowIfNot_SOK(this->audioDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, nullptr, (void**)&this->audioSessionManager));
	}

	AudioDevice::AudioDevice(const AudioDevice& other)
	{
		this->audioDevice = other.audioDevice;
		this->audioDevice->AddRef();
		this->audioMeterInformation = other.audioMeterInformation;
		this->audioMeterInformation->AddRef();
		this->audioSessionManager = other.audioSessionManager;
		this->audioSessionManager->AddRef();
	}
	
	AudioDevice::~AudioDevice()
	{
		this->audioDevice->Release();
		// release
	}

	AudioDevice AudioDevice::GetDefaultAudioDevice() 
	{
		CComPtr<IMMDevice> audioDevice;
		CComPtr<IMMDeviceEnumerator> audioDeviceEnumerator;
			
		__HR_AudioDevice::ThrowIfNot_SOK(audioDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL));		
		__HR_AudioDevice::ThrowIfNot_SOK(audioDeviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eConsole, &audioDevice));
			
		return AudioDevice(audioDevice.Detach());
	}

	AudioDevice AudioDevice::GetAudioDeviceById(const wstring& id) 
	{
		CComPtr<IMMDevice> audioDevice;
		CComPtr<IMMDeviceEnumerator> audioDeviceEnumerator;
			
		__HR_AudioDevice::ThrowIfNot_SOK(audioDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL));		
		__HR_AudioDevice::ThrowIfNot_SOK(audioDeviceEnumerator->GetDevice(id.c_str(), &audioDevice));
			
		return AudioDevice(audioDevice.Detach());
	}

	vector<AudioDevice> AudioDevice::GetAudioDevices()
	{
		vector<AudioDevice> devices;

		CComPtr<IMMDeviceEnumerator> audioDeviceEnumerator;
		__HR_AudioDevice::ThrowIfNot_SOK(audioDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL));		

		CComPtr<IMMDeviceCollection> audioDeviceCollection;		
		__HR_AudioDevice::ThrowIfNot_SOK(audioDeviceEnumerator->EnumAudioEndpoints(EDataFlow::eRender, DEVICE_STATE_ACTIVE, &audioDeviceCollection));

		UINT count = 0;

		__HR_AudioDevice::ThrowIfNot_SOK(audioDeviceCollection->GetCount(&count));

		for (UINT i = 0; i < count; i++)
		{
			CComPtr<IMMDevice> device;
			__HR_AudioDevice::ThrowIfNot_SOK(audioDeviceCollection->Item(i, &device));
			devices.push_back(AudioDevice(device.Detach()));
		}

		return devices;
	}

	wstring AudioDevice::GetId()
	{
		wstring id;
		LPWSTR pwszId = NULL;

		HRESULT hr = this->audioDevice->GetId(&pwszId);		
		if (FAILED(hr))
		{
			::CoTaskMemFree(pwszId);
			__HR_AudioDevice::Throw(hr);
		}

		id = wstring(pwszId);		
		::CoTaskMemFree(pwszId);

		return id;
	}

	wstring AudioDevice::GetName()
	{
		wstring deviceName;

		CComPtr<IPropertyStore> propertyStore;			

		// Open the property store
		__HR_AudioDevice::ThrowIfNot_SOK(this->audioDevice->OpenPropertyStore(STGM_READ, &propertyStore));
			
		// Get the friendly name of the endpoint device, e.g. "Speakers (XYZ Audio Adapter)"
		PROPERTYKEY propertyKey = PKEY_Device_FriendlyName;
		PROPVARIANT propertyValue;
		::PropVariantInit(&propertyValue);
		HRESULT hr = propertyStore->GetValue(propertyKey, &propertyValue);
		if (FAILED(hr))
		{
			::PropVariantClear(&propertyValue);
			__HR_AudioDevice::Throw(hr);
		}
		
		deviceName = wstring(propertyValue.pwszVal);
		
		__HR_AudioDevice::ThrowIfNot_SOK(::PropVariantClear(&propertyValue));

		return deviceName;
	}

	float AudioDevice::GetPeakAudioLevel()
	{
		float peakLevel;
		// The reported value is normalized to the range from 0.0 to 1.0,
		// for a PCM stream containing 16-bit samples with a peak sample of -8914
		// the reported value will be 8914/32768 = 0.272
		__HR_AudioDevice::ThrowIfNot_SOK(this->audioMeterInformation->GetPeakValue(&peakLevel));
		// Convert the value to a range from 0 to 100 for convenience
		return peakLevel * 100;
	}

	vector<AudioSession> AudioDevice::GetAudioSessions()
	{
		vector<AudioSession> sessions;

		CComPtr<IAudioSessionEnumerator> sessionEnumerator;

		__HR_AudioDevice::ThrowIfNot_SOK(this->audioSessionManager->GetSessionEnumerator(&sessionEnumerator));

		int sessionsCount = 0;

		__HR_AudioDevice::ThrowIfNot_SOK(sessionEnumerator->GetCount(&sessionsCount));

		for (int i = 0; i < sessionsCount; i++)
		{
			CComPtr<IAudioSessionControl> sessionControl;
			__HR_AudioDevice::ThrowIfNot_SOK(sessionEnumerator->GetSession(i, &sessionControl));				
			CComQIPtr<IAudioSessionControl2> sessionControl2 = sessionControl;				
			sessions.push_back(AudioSession(sessionControl2));
		}

		return sessions;
	}
}