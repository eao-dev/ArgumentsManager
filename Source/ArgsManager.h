#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "InvalidArg.h"

/**
	@mainpage
	A small C++ library for convenient management of command line arguments.
	Allows registration of required arguments, sets of required arguments,
	help arguments, optional arguments, performs parsing and validation.

	To use the library, you need to copy it, add it to the project and include the ArgsManager.h file
*/

/**
	@brief
	The class representing the argument.
	The class contains information about the potential input argument.
	Each instance can be associated with one or two strings (for example: '-i' and '--input')
*/
class Argument {

private:

	const std::string arg1;
	const std::string arg2;
	static constexpr const char* emptyArgsErrorMsg = "Add argument cannot be empty!";
	bool has_Content = false;

public:

	// TODO: add move semantics constructor.

	/**
		@brief constructor.
		@throw If arg1 is empty.
		@param arg1 argument (required).
	*/
	Argument(const char* const arg1) :
		arg1(arg1) {
		if (arg1 == nullptr || std::string(arg1).empty())
			throw std::invalid_argument(emptyArgsErrorMsg);
	}

	/**
		@brief constructor.
		@throw If arg1 is empty.
		@param arg1 argument (required).
		@param arg2 argument (optional).
	*/
	Argument(const std::string& arg1, const std::string& arg2 = std::string()) :
		arg1(arg1), arg2(arg2) {
		if (arg1.empty())
			throw std::invalid_argument(emptyArgsErrorMsg);
	}

	/**
		@brief constructor.
		@param hasContent flag indicating whether the argument contains content.
		@throw If arg1 is empty.
		@param arg1 argument (required).
		@param arg2 argument (optional).
	*/
	Argument(bool hasContent, const std::string& arg1, const std::string& arg2 = std::string()):
		arg1(arg1), arg2(arg2), has_Content(hasContent) {
		if (arg1.empty())
			throw std::invalid_argument(emptyArgsErrorMsg);
	}

	/**
		@brief constructor.
		@param hasContent flag indicating whether the argument contains content.
		@param arg1 argument (required).
		@param arg2 argument (optional).
	*/
	Argument(bool hasContent, std::string&& arg1, std::string&& arg2 = std::string()):
		arg1(std::move(arg1)), arg2(std::move(arg2)), has_Content(hasContent) {}

	Argument(const Argument&) = default;
	Argument& operator=(const Argument&) = default;

	/**
		@brief Returns argument 1.
	*/
	const std::string& getArg1() const {
		return arg1;
	};

	/**
		@brief Returns argument 2.
	*/
	const std::string& getArg2() const {
		return arg2;
	};

	/**
		@brief Returns TRUE if content should be passed to the argument, otherwise FALSE.
	*/
	bool hasContent() const {
		return has_Content;
	}

	/**
		@brief Returns TRUE if argument 1 and argument 2 are the same, otherwise FALSE.
		@param arg instance of Argument.
	*/
	bool operator==(const Argument& arg) const noexcept {
		return (getArg1() == arg.getArg1() && getArg2() == arg.getArg2());
	}

	/**
		@brief Returns TRUE if argument 1 or argument 2 is the same as @arg, FALSE otherwise.
		@param arg argument.
	*/
	bool operator==(const std::string& arg) const noexcept {
		return (getArg1() == arg || getArg2() == arg);
	}

	/**
		@brief Returns TRUE if argument 1 or argument 2 is the same as @arg, FALSE otherwise.
		@throw If arg is NULL.
		@param arg argument.
	*/
	bool operator==(const char* const arg) const {
		if (arg == nullptr)
			throw std::invalid_argument("arg is NULL");
		return (getArg1() == arg || getArg2() == arg);
	}
};

namespace std {
	template<> 
	/**
		Hash implementation for Argument
	*/
	struct hash<Argument> {
		std::size_t operator()(const Argument& value) const noexcept {
			hash<string> hash;
			const std::size_t hashValue1 = hash(value.getArg1());
			const std::size_t hashValue2 = hash(value.getArg2());
			return hashValue1 ^ (hashValue2 << 1);
		}
	};
}

class ArgsManager;

using ArgContent = std::string;
using ArgContentMap = std::unordered_map<Argument, ArgContent>;

/**
	@brief
	Provides options for registering arguments, parsing them, validating them, extracting content.
*/
class ArgsManager
{

private:

	ArgsManager() = default;	
	std::unordered_set<Argument>
		  requiredArgs
		, optionalArgs
		, requiredArgSet;

	ArgContent getContentForArg(const Argument& argument, 
		const unsigned int argc, const unsigned int idx, const char* const argv[]) const;

	std::unordered_set<std::string> helpArgs;

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
		@return instance of this calss.
		@param optionalArg added optional argument.
	*/
	ArgsManager& addOptional(const Argument& optionalArg);

	/**
		@brief Clear the set of all argument.
	*/
	void clearAllParams();

	/**
		@brief Performs parsing of passed arguments, validation of input arguments, and extraction of argument values.
		@return Returns a structure containing the values associated with the arguments.
		@throw If argc == 0, beginIdx > argc, argv is NULL pointer.
		@param argc count of arguments.
		@param argv arguments array.
		@beginIdx   initial argument number.
	*/
	ArgContentMap parse(const unsigned int argc, const char* const argv[], unsigned int beginIdx) const;

	/**
		@brief Extract content from instance of ArgContentMap.
		@return Instance of ArgContent, contains the conent for the specified argument.
		@throw arg not found or has no content.
		@param argContentMap set of options and content.
		@param arg argument for which the content should be retrieved.
	*/
	ArgContent argValue(const ArgContentMap& argContentMap, const Argument& arg) const;

	/**
		@brief Checks if input parameters are parameters for outputting help.
		@return Returns true if argv contains a help parameter, false otherwise.
		@throw If argc == 0, beginIdx > argc, argv is NULL pointer.
		@param argc count of arguments.
		@param argv arguments array.
		@beginIdx   initial argument number.
	*/
	bool isHelpArg(const unsigned int argc, const char* const argv[], unsigned int beginIdx) const;
};