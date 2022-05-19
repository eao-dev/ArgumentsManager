#pragma once

#include <exception>

class InvalidParameter: public std::exception
{
	
public:
	InvalidParameter(const std::string& errorMsg) : std::exception(errorMsg.c_str()) {};
	virtual ~InvalidParameter() {};
};





