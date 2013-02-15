#pragma once

#include "Exception.h"

#include <string>

#include <Windows.h>
#include <comdef.h>

#define ThrowIfNot_SOK(hr) { if (FAILED(hr)) throw ComException(hr); }

namespace recgzer_core {

	class ComException : public Exception
	{
	public:
		ComException(HRESULT hr) : hr(hr), Exception(_com_error(hr).ErrorMessage()) {}
		ComException(HRESULT hr, const std::wstring& message) : hr(hr), Exception(message) {}

		ComException(const ComException& other) : hr(other.hr), Exception(other) {}

		ComException& operator=(const ComException& other)
		{
			Exception::operator=(other);
			this->hr = other.hr;
			return *this;
		}

	public:
		HRESULT Hr() const
		{
			return this->hr;
		}

	private:
		HRESULT hr;
	};
}