#include <string>

namespace recgzer_core {

	struct AudioDeviceId
	{
	public:
		AudioDeviceId(std::wstring id) : Id(id) {}
	public:
		const std::wstring Id;
	};
}