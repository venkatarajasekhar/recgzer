#pragma once

#include "IAudioStream.h"

#include <memory>

namespace recgzer_core {

	class IAudioRecorder
	{
	public:
		virtual void Start() = 0;
		virtual void Stop() = 0;
		virtual int Length() = 0;
		virtual std::unique_ptr<IAudioStream> Stream() = 0;
	};
}
