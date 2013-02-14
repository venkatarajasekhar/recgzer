#include <string>

namespace recgzer_core {

	struct AudioDeviceId
	{
	public:
		AudioDeviceId(std::wstring id, std::wstring name) : Id(id), Name(name) {}

	public:
		const std::wstring Id;
		const std::wstring Name;
	};
}