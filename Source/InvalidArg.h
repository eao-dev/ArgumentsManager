#pragma once

#include <exception>

/**
	@brief
	Class-exception.
*/
class InvalidArg: public std::exception
{
	
public:

	/**
		@brief constructor.
	*/
	InvalidArg(const std::string& errorMsg) : std::exception(errorMsg.c_str()) {};
	virtual ~InvalidArg() {};
};