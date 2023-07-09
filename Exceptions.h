#pragma once
#include <exception>
#include <string>
#include <sstream>

namespace except {
	class BaseException : public std::exception
	{
	public:
		BaseException(int line, const char* filename);
		const char* what() const override;
		virtual const char* getType() const;
		int getLine() const;
		const std::string getFilename() const;
		std::string getOrigin() const;
	private:
		int line = 0;
		std::string filename;
	protected:
		mutable std::string written;
	};
}