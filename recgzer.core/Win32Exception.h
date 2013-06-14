#pragma once

#include "Exception.h"

#include <string>

#include <Windows.h>
#include <comdef.h>

namespace recgzer_core {

	template <class Result> Result ThrowIfFailed(Result r)
	{
		if (!r)
		{
			throw Win32Exception(GetLastError());
		}

		return r;
	}

	template<typename Result> Result ThrowCheckIfFailed(Result r)
	{
		if (!r)
		{
			// NOTE: assumes the Win32 function implements GetLastError.
			// Here we optimize for the successful case, it's possible the Win32 call
			// returned negative but that might be because of the way messages are handled,
			// or that errors are not actually returned, the call being successful, EX: SendMessage.
			DWORD err = GetLastError();
			if (err != 0)
			{
				throw Win32Exception(err);
			}
		}

		return r;
	}

	class Win32Exception : public Exception
	{
	public:
		Win32Exception(DWORD error) : error(error), Exception(_com_error(HRESULT_FROM_WIN32(error)).ErrorMessage()) {}
		Win32Exception(DWORD error, const std::wstring& message) : error(error), Exception(message) {}
		Win32Exception(const std::wstring& message) : error(GetLastError()), Exception(message) {}

		Win32Exception(const Win32Exception& other) : error(other.error), Exception(other) {}

		Win32Exception& operator=(const Win32Exception& other)
		{
			Exception::operator=(other);
			this->error = other.error;
			return *this;
		}

	public:
		DWORD Error() const
		{
			return this->error;
		}

	private:
		DWORD error;
	};
}