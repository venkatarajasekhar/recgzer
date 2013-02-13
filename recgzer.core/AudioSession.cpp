#include "AudioSession.h"

#include "__HR_AudioDevice.h"
#include "ProcessInfo.h"
#include "AudioSessionState.h"

#include <Audiopolicy.h>

namespace YoutubeScrobbler {
	
	AudioSession::AudioSession(IAudioSessionControl2* sessionControl2)
	{	
		__HR_AudioDevice::ThrowIfNot_SOK(sessionControl2->GetState((::AudioSessionState*)&this->state));
		__HR_AudioDevice::ThrowIfNot_SOK(sessionControl2->GetProcessId(&this->ownerProcessId));
	}

	ProcessInfo AudioSession::GetOwnerProcess()
	{
		return ProcessInfo(this->ownerProcessId);
	}
}