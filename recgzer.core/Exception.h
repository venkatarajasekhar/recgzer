#pragma once

#include <exception>
#include <string>

#include <atlbase.h>

namespace recgzer_core {

	inline std::wstring ToUtf8(_In_z_ const char* message) { return std::wstring(CA2WEX<>(message, CP_UTF8).m_psz); }
	inline std::wstring ToUtf8(const std::string& message) { return std::wstring(CA2WEX<>(message.c_str(), CP_UTF8).m_psz); }
	inline std::string ToAnsi(_In_z_ const wchar_t* message) { return std::string(CW2AEX<>(message, CP_ACP).m_psz); }
	inline std::string ToAnsi(const std::wstring& message) { return std::string(CW2AEX<>(message.c_str(), CP_ACP).m_psz); }
}

namespace recgzer_core {

	class Exception : public std::exception
	{
	public:
		Exception() : std::exception() {}

		explicit Exception(const std::wstring& message) : message(message), std::exception(ToAnsi(message).c_str()) {}

		Exception(const Exception& other) : std::exception(other) { *this = other; }

		Exception(Exception&& other) : std::exception(other) { *this = std::move(other); }

		Exception& operator=(const Exception& other)
		{
			std::exception::operator=(other);
			this->message = other.message;
			return *this;
		}

		Exception& operator=(Exception&& other)
		{
			std::exception::operator=(other);
			this->message = std::move(other.message);
			return *this;
		}

	public:
		virtual const std::wstring& Message() const
		{
			return this->message;
		}

	private:
		std::wstring message;
	};
}