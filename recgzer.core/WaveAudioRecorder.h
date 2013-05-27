#pragma once

#include "IAudioRecorder.h"

#include <Mmdeviceapi.h>
#include <Audioclient.h>
//#include <Functiondiscoverykeys_devpkey.h>
//#include <Mmreg.h>
//#include <Ksmedia.h>

#include <atlbase.h>

#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC  10000

namespace recgzer_core {
	// stream, ....
	class WaveAudioRecorder final : public IAudioRecorder
	{
	friend class AudioDevice;

	private:
		WaveAudioRecorder(IMMDevice*);

	public:
		WaveAudioRecorder();

	public:
		virtual void Start();
		virtual void Stop();

	public:
		virtual int Length();
		virtual std::unique_ptr<IAudioStream> Stream();

	private:
		CComPtr<IAudioClient> audioClient;
	};
}