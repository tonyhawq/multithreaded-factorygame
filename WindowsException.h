#pragma once

#include "Winbrew.h"
#include "Exceptions.h"

class except::WindowException : public except::BaseException
{
public:
	WindowException(int line, const char* file, HRESULT hr);
	const char* what() const override;
	const char* getType() const override;
	static std::string translateErrorCode(HRESULT hr);
	HRESULT getErrorCode() const;
	std::string getErrorString() const;
private:
	HRESULT hResult;
};