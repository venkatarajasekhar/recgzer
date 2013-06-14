#pragma once

#include "IAudioRecorder.h"

#include "ComException.h"
#include "Exception.h"

#include <Mmdeviceapi.h>
#include <Audioclient.h>
//#include <Functiondiscoverykeys_devpkey.h>
//#include <Mmreg.h>
//#include <Ksmedia.h>

#include <atlbase.h>

#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC  10000

namespace recgzer_core {

	class WaveAudioRecorder final : public IAudioRecorder
	{
	friend class AudioDevice;

	private:
		WaveAudioRecorder(IMMDevice*);

	public:
		WaveAudioRecorder();

	public:
		virtual void Start()
		{
		}

		virtual void Stop()
		{
		}

	public:
		virtual int Length();
		virtual std::unique_ptr<IAudioStream> Stream();

	//protected:
	public:
		void Initialize()
		{
			WAVEFORMATEX* format;
			ThrowIfNot_SOK(audioClient->GetMixFormat(&format));

			switch (format->wFormatTag)
			{
			case WAVE_FORMAT_IEEE_FLOAT:
				{
					format->wFormatTag = WAVE_FORMAT_PCM;
					format->wBitsPerSample = 16;
					format->nBlockAlign = format->nChannels * format->wBitsPerSample / 8;
					format->nAvgBytesPerSec = format->nBlockAlign * format->nSamplesPerSec;
				}
				break;

			case WAVE_FORMAT_EXTENSIBLE:
				{
					PWAVEFORMATEXTENSIBLE formatEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(format);
					if (::IsEqualGUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, formatEx->SubFormat))
					{
						formatEx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
						formatEx->Samples.wValidBitsPerSample = 16;
						format->wBitsPerSample = 16;
						format->nBlockAlign = format->nChannels * format->wBitsPerSample / 8;
						format->nAvgBytesPerSec = format->nBlockAlign * format->nSamplesPerSec;
					}
				}
				break;

			default:
				throw Exception(L"Unsupported format");
			}

			WAVEFORMATEX* supportedFormat;
			ThrowIfNot_SOK(audioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, format, &supportedFormat));
			
			// TODO: check against the format we asked for

			REFERENCE_TIME hnsDefaultDevicePeriod;
			ThrowIfNot_SOK(audioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL));
			LONG sleepms = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000);

			ThrowIfNot_SOK(audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, format, NULL)); 

			ThrowIfNot_SOK(audioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&this->audioCaptureClient));

			
		}

	private:
		void RecordingLoop()
		{
		}

	private:
		CComPtr<IAudioClient> audioClient;
		CComPtr<IAudioCaptureClient> audioCaptureClient;
	};
}