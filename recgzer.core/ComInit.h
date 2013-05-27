#include "ComException.h"

#include <Windows.h>

namespace recgzer_core {

	class ComInit
	{
	public:
		ComInit(COINIT initType = COINIT_MULTITHREADED)
		{
			ThrowIfNot_SOK(::CoInitializeEx(NULL, initType));
		}

		~ComInit()
		{
			::CoUninitialize();
		}
	};
}