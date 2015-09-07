#include <string>

#include <wchar.h>

namespace recgzer_core {

	class AudioDeviceId
	{
	public:
	        const std::wstring Id;
		AudioDeviceId(std::wstring id) : Id(id) {}
		bool operator == (const AudioDeviceId& id) const { return this->EqualsTo(id); }
		bool operator != (const AudioDeviceId& id) const { return this->EqualsTo(id); }

	private:
		bool EqualsTo(const AudioDeviceId& id) const
		{
			if (wcscmp(this->Id.c_str(), id.Id.c_str()) == 0)
				return true;
			return false;
		}

	
	};
}
