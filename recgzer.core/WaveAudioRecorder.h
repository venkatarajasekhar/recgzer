#pragma once

#include "IAudioRecorder.h"

#include "ComException.h"
#include "Win32Exception.h"
#include "Exception.h"

#include <Mmdeviceapi.h>
#include <Audioclient.h>
//#include <Functiondiscoverykeys_devpkey.h>
//#include <Mmreg.h>
//#include <Ksmedia.h>

#include <atlbase.h>

#include <avrt.h>

#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC  10000

#define TASK_NAME L"Pro Audio"

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
#if 1 // format handling
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
#endif
			
			REFERENCE_TIME hnsDefaultDevicePeriod;
			ThrowIfNot_SOK(audioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL));
			LONG sleepms = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000);

			ThrowIfNot_SOK(audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, 0, 0, format, NULL)); 

			HANDLE signal = ThrowIfFailed(::CreateEventA(NULL, FALSE, FALSE, NULL));
			
			

			//if () {}


			ThrowIfNot_SOK(audioClient->SetEventHandle(signal));
						
			ThrowIfNot_SOK(audioClient->GetService(__uuidof(IAudioRenderClient), (void**)&this->audioRenderClient));
			
			/*
			// To reduce latency, load the first buffer with data
			// from the audio source before starting the stream.
			hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
			EXIT_ON_ERROR(hr)

			hr = pMySource->LoadData(bufferFrameCount, pData, &flags);
			EXIT_ON_ERROR(hr)

			hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
			EXIT_ON_ERROR(hr)
			*/

			HANDLE hTask;
			DWORD taskIndex = 0;
			hTask = AvSetMmThreadCharacteristicsW(L"Pro Audio", &taskIndex);
			/*if (hTask == NULL)
			{
				hr = E_FAIL;
				EXIT_ON_ERROR(hr)
			}*/

			DWORD waitState = ::WaitForSingleObject(signal, 1000);

			/*
			// Wait for the last buffer to play before stopping.
			Sleep((DWORD)(hnsRequestedDuration/REFTIMES_PER_MILLISEC));
			*/
		}

	private:
		void RecordingLoop()
		{
		}

	private:
		CComPtr<IAudioClient> audioClient;
		CComPtr<IAudioCaptureClient> audioCaptureClient;
		CComPtr<IAudioRenderClient> audioRenderClient;
	};
}