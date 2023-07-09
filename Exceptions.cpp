#include "Exceptions.h"

except::BaseException::BaseException(int line, const char* filename) {
	this->line = line;
	this->filename = filename;
}

const char* except::BaseException::what() const {
	std::ostringstream stringstream;
	stringstream << getType() << '\n'
		<< getOrigin();
	written = stringstream.str();
	return written.c_str();
}

const char* except::BaseException::getType() const {
	return "Base exception";
}

int except::BaseException::getLine() const {
	return line;
}

const std::string except::BaseException::getFilename() const {
	return filename;
}

std::string except::BaseException::getOrigin() const {
	std::ostringstream stringstream;
	stringstream << "In File: " << filename << '\n'
		<< "At Line: " << line;
	return stringstream.str();
}