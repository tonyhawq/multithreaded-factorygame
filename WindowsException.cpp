#include "WindowsException.h"

except::WindowException::WindowException(int line, const char* file, HRESULT hr)
	: except::BaseException(line, file)
{
	hResult = hr;
}

const char* except::WindowException::what() const {
	std::ostringstream stringstream;
	stringstream << this->getType() << '\n'
		<< "Error code: " << this->getErrorCode() << '\n'
		<< "Error Desc: " << this->getErrorString() << '\n'
		<< getOrigin();
	written = stringstream.str();
	return written.c_str();
}

const char* except::WindowException::getType() const {
	return "Window Exception";
}

std::string except::WindowException::translateErrorCode(HRESULT hr) {
	char* messageBuffer = NULL;
	DWORD msgLength = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer),
		0,
		NULL
	);
	if (msgLength == 0)
	{
		return "Something went wrong during formatting";
	}
	std::string errorString = messageBuffer;
	LocalFree(messageBuffer);
	return errorString;
}

HRESULT except::WindowException::getErrorCode() const {
	return hResult;
}

std::string except::WindowException::getErrorString() const {
	return this->translateErrorCode(hResult);
}