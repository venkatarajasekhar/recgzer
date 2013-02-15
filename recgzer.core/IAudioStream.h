#pragma once

#include <vector>

namespace recgzer_core {

	class IAudioStream
	{
	public:
		virtual int Size() const = 0;
		virtual std::vector<unsigned char> ReadPacket(int, int) const = 0;
	};
}