#pragma once
#include "ArgsManager.h"

/**
	@brief
	The class representing the argument.
	The class contains information about the potential input argument.
	Each instance can be associated with one or two strings (for example: '-i' and '--input')
*/
class Argument {

private:

	std::string arg1;
	std::string arg2;
	static constexpr const char* emptyArgsErrorMsg = "Add argument cannot be empty!";
	bool has_Content = false;

public:

	/**
		@brief constructor.
		@throw If arg1 is empty.
		@param arg1 argument (required).
	*/
	Argument(const char* const arg1);

	/**
		@brief constructor.
		@throw If arg1 is empty.
		@param arg1 argument (required).
		@param arg2 argument (optional).
	*/
	Argument(const std::string& arg1, const std::string& arg2 = std::string());

	/**
		@brief constructor.
		@param hasContent flag indicating whether the argument contains content.
		@throw If arg1 is empty.
		@param arg1 argument (required).
		@param arg2 argument (optional).
	*/
	Argument(bool hasContent, const std::string& arg1, const std::string& arg2 = std::string());

	/**
		@brief constructor.
		@param hasContent flag indicating whether the argument contains content.
		@param arg1 argument (required).
		@param arg2 argument (optional).
	*/
	Argument(bool hasContent, std::string&& arg1, std::string&& arg2 = std::string());

	/**
		@brief Move semantics constructor.
	*/
	Argument(Argument&& arg) noexcept;

	/**
		@brief Move semantics operator=.
	*/
	Argument& operator=(Argument&& arg) noexcept;

	Argument(const Argument&) = default;
	Argument& operator=(const Argument&) = default;

	/**
		@brief Returns argument 1.
	*/
	const std::string& getArg1() const;

	/**
		@brief Returns argument 2.
	*/
	const std::string& getArg2() const;

	/**
		@brief Returns TRUE if content should be passed to the argument, otherwise FALSE.
	*/
	bool hasContent() const;

	/**
		@brief Returns TRUE if at least one argument matches, otherwise FALSE.
		@param arg instance of Argument.
	*/
	bool operator==(const Argument& arg) const noexcept;

	/**
		@brief Returns TRUE if at least one argument matches, FALSE otherwise.
		Example:
			Argument(false, "-a", "-b") == Argument(false, "-a")
			Argument(false, "-a", "-b") == Argument(false, "-b")
		@param arg argument.
	*/
	bool operator==(const std::string& arg) const noexcept;
};