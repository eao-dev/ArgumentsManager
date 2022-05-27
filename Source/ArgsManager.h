#pragma once

#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "InvalidArg.h"
#include "Argument.h"

/**
	@mainpage
	A small C++ library for convenient management of command line arguments.
	Allows registration of required arguments, sets of required arguments,
	help arguments, optional arguments, performs parsing and validation.

	To use the library, you need to copy it, add it to the project and include the ArgsManager.h file
*/

using Content = std::string;
struct ArgContent{Argument arg; Content content;};
using ArgContentList = std::list<ArgContent>;

/**
	@brief
	Provides options for registering arguments, parsing them, validating them, extracting content.
	The class implements the singleton pattern.
*/
class ArgsManager
{

private:

	ArgsManager() = default;
	std::vector<Argument>
		requiredArgs
		, optionalArgs
		, requiredArgSet;

	ArgContentList argContentList;

	Content getContent(const Argument& argument,
		const unsigned int argc, const unsigned int idx, const char* const argv[]) const;

	std::unordered_set<std::string> helpArgs;
	bool parsed = false;

	bool checkExists(const Argument& argument);

public:
	ArgsManager(const ArgsManager&) = delete;
	ArgsManager(const ArgsManager&&) = delete;
	void operator=(const ArgsManager&) = delete;
	void operator=(const ArgsManager&&) = delete;

	/**
		@brief Returns an instance of this class.
		@return instance of this calss.
	*/
	static ArgsManager& getInstance();
	virtual ~ArgsManager();

	/**
		@brief Add of options responsible for displaying help.
		@return instance of this calss.
		@param helpParam argument.
	*/
	ArgsManager& addHelp(const std::string& helpParam);

	/**
		@brief Add required argument.
		@return instance of this calss.
		@param arg added required argument.
	*/
	ArgsManager& addRequired(const Argument& arg);

	/**
		@brief Add a required argument.
		@return instance of this calss.
		At least one argument added with this method must be passed.
		@param arg added required argument.
	*/
	ArgsManager& addRequiredToSet(const Argument& arg);

	/**
		@brief Add an optional argument.
		@return Instance of this calss.
		@param optionalArg added optional argument.
	*/
	ArgsManager& addOptional(const Argument& optionalArg);

	/**
		@brief Clear the set of all argument.
	*/
	void clear();

	/**
		@brief Performs parsing of passed arguments, validation of input arguments, and extraction of argument values.
		@throw If argc == 0, beginIdx > argc, argv is NULL pointer.
		@param argc count of arguments.
		@param argv arguments array.
		@beginIdx   Initial argument number.
	*/
	void parse(const unsigned int argc, const char* const argv[], unsigned int beginIdx);

	/**
		@brief Extract content from instance of ArgContentMap. Method parse() must be called before this method.
		@return Instance of ArgContent, contains the conent for the specified argument.
		@throw arg Not found, has no content or method parse() was not called.
		@param arg argument for which the content should be retrieved.
	*/
	Content argValue(const Argument& arg) const;

	/**
		@brief Checks if the argument is present in the passed arguments.
		@return True if arguemnt is present in the passed arguments, otherwise false.
		@throw If method parse() was not called.
		@param arg Argument.
	*/
	bool argPresent(const Argument& arg) const;

	/**
		@brief Checks if input parameters are parameters for outputting help. Method parse() must be called before this method.
		@return Returns true if argv contains a help parameter, false otherwise.
		@throw If argc == 0, beginIdx > argc, argv is NULL pointer.
		@param argc count of arguments.
		@param argv arguments array.
		@beginIdx   initial argument number.
	*/
	bool isHelpArg(const unsigned int argc, const char* const argv[], unsigned int beginIdx) const;
};