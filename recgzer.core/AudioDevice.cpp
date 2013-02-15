#include "AudioDevice.h"

#include "Exception.h"
#include "ComException.h"
#include "IAudioRecorder.h"
#include "WaveAudioRecorder.h"

#include <vector>

#include <mmdeviceapi.h>
#include <Endpointvolume.h>
#include <Audiopolicy.h>
#include <Functiondiscoverykeys_devpkey.h>

#include <atlbase.h>

namespace recgzer_core {

	/***********************************************************************
	* Object construction
	***********************************************************************/

	AudioDevice::AudioDevice() {}

	AudioDevice::~AudioDevice() {}

	AudioDevice::AudioDevice(IMMDevice* const audioDevice)
	{
		this->audioDevice = audioDevice;
		ThrowIfNot_SOK(this->audioDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, nullptr, (void**)&this->audioMeterInformation));
		ThrowIfNot_SOK(this->audioDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, nullptr, (void**)&this->audioSessionManager));
	}

	AudioDevice::AudioDevice(const AudioDevice& other)
	{
		this->audioDevice = other.audioDevice;
		this->audioMeterInformation = other.audioMeterInformation;
		this->audioSessionManager = other.audioSessionManager;
	}

	/***********************************************************************
	* Factories
	***********************************************************************/

	AudioDevice AudioDevice::DefaultAudioDevice()
	{
		CComPtr<IMMDevice> audioDevice;
		CComPtr<IMMDeviceEnumerator> audioDeviceEnumerator;
			
		HRESULT hr = audioDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL);
		ThrowIfNot_SOK(audioDeviceEnumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eConsole, &audioDevice));
			
		return AudioDevice(audioDevice.Detach());
	}

	AudioDevice AudioDevice::GetAudioDeviceById(const AudioDeviceId& id) 
	{
		CComPtr<IMMDevice> audioDevice;
		CComPtr<IMMDeviceEnumerator> audioDeviceEnumerator;
			
		ThrowIfNot_SOK(audioDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL));
		ThrowIfNot_SOK(audioDeviceEnumerator->GetDevice(id.Id.c_str(), &audioDevice));
			
		return AudioDevice(audioDevice.Detach());
	}

	std::vector<AudioDevice> AudioDevice::AudioDevices()
	{
		std::vector<AudioDevice> devices;

		CComPtr<IMMDeviceEnumerator> audioDeviceEnumerator;
		ThrowIfNot_SOK(audioDeviceEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL));

		CComPtr<IMMDeviceCollection> audioDeviceCollection;
		ThrowIfNot_SOK(audioDeviceEnumerator->EnumAudioEndpoints(EDataFlow::eRender, DEVICE_STATE_ACTIVE, &audioDeviceCollection));

		UINT count = 0;

		ThrowIfNot_SOK(audioDeviceCollection->GetCount(&count));

		for (UINT i = 0; i < count; i++)
		{
			CComPtr<IMMDevice> device;
			ThrowIfNot_SOK(audioDeviceCollection->Item(i, &device));
			devices.push_back(AudioDevice(device.Detach()));
		}

		return devices;
	}

	/***********************************************************************
	* Properties
	***********************************************************************/

	AudioDeviceId AudioDevice::Id() const
	{
		std::wstring id;
		LPWSTR pwszId = NULL;

		HRESULT hr = this->audioDevice->GetId(&pwszId);
		if (FAILED(hr))
		{
			::CoTaskMemFree(pwszId);
			throw ComException(hr);
		}

		id = std::wstring(pwszId);
		::CoTaskMemFree(pwszId);
		
		return AudioDeviceId(id);
	}

	std::wstring AudioDevice::DisplayName() const
	{
		std::wstring deviceName;

		CComPtr<IPropertyStore> propertyStore;

		// Open the property store
		ThrowIfNot_SOK(this->audioDevice->OpenPropertyStore(STGM_READ, &propertyStore));

		// Get the friendly name of the endpoint device, e.g. "Speakers (XYZ Audio Adapter)"
		PROPERTYKEY propertyKey = PKEY_Device_FriendlyName;
		PROPVARIANT propertyValue;
		::PropVariantInit(&propertyValue);
		HRESULT hr = propertyStore->GetValue(propertyKey, &propertyValue);
		if (FAILED(hr))
		{
			::PropVariantClear(&propertyValue);
			throw ComException(hr);
		}
		
		deviceName = std::wstring(propertyValue.pwszVal);

		ThrowIfNot_SOK(::PropVariantClear(&propertyValue));

		return deviceName;
	}

	float AudioDevice::PeakAudioLevel() const
	{
		float peakLevel = 0;
		// The reported value is normalized to the range from 0.0 to 1.0,
		// for a PCM stream containing 16-bit samples with a peak sample of -8914
		// the reported value will be 8914/32768 = 0.272
		ThrowIfNot_SOK(this->audioMeterInformation->GetPeakValue(&peakLevel));
		// Convert the value to a range from 0 to 100 for convenience
		return peakLevel * 100;
	}

	std::unique_ptr<IAudioRecorder> AudioDevice::Recorder(AudioRecorderType type) const
	{
		switch (type)
		{
		case AudioRecorderType::Wave:
			return std::unique_ptr<IAudioRecorder>(new WaveAudioRecorder(this->audioDevice.p));
		}

		throw Exception(L"Cannot create recorder. Unsupported recorder type.");
	}
}