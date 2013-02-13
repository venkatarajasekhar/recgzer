#pragma once

#include "ProcessInfo.h"
#include "AudioSessionState.h"

struct IAudioSessionControl2;

namespace YoutubeScrobbler {

	class AudioSession
	{
	friend class AudioDevice;

	private:
		AudioSession();
		AudioSession(IAudioSessionControl2* sessionControl2);

	public:
		AudioSession(const AudioSession& other) { *this = other; }
		AudioSession& operator=(const AudioSession& other)
		{
			this->state = other.state;
			this->ownerProcessId = other.ownerProcessId;
			return *this;
		}
		~AudioSession() {}

	public:
		// Gets the state of this audio session.
		AudioSessionState GetState() { return this->state; }
		// Gets a ProcessInfo object which incapsulates information about the process who owns the audio session.
		ProcessInfo GetOwnerProcess();

	private:
		AudioSessionState state;
		unsigned long ownerProcessId;
	};
}