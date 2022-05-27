#include "ArgsManager.h"

Argument::Argument(const char* const arg1) :
	arg1(arg1) {
	if (arg1 == nullptr || std::string(arg1).empty())
		throw std::invalid_argument(emptyArgsErrorMsg);
}

Argument::Argument(const std::string& arg1, const std::string& arg2) :
	arg1(arg1), arg2(arg2) {
	if (arg1.empty())
		throw std::invalid_argument(emptyArgsErrorMsg);
}

Argument::Argument(bool hasContent, const std::string& arg1, const std::string& arg2) :
	arg1(arg1), arg2(arg2), has_Content(hasContent) {
	if (arg1.empty())
		throw std::invalid_argument(emptyArgsErrorMsg);
}

Argument::Argument(bool hasContent, std::string&& arg1, std::string&& arg2) :
	arg1(std::move(arg1)), arg2(std::move(arg2)), has_Content(hasContent) {}

Argument::Argument(Argument&& arg) noexcept {
	*this = std::move(arg);
}

Argument& Argument::operator=(Argument&& arg) noexcept {
	arg1 = std::move(arg.arg1);
	arg2 = std::move(arg.arg2);
	has_Content = arg.has_Content;
	arg.has_Content = false;
	return *this;
}

const std::string& Argument::getArg1() const {
	return arg1;
};

const std::string& Argument::getArg2() const {
	return arg2;
};

bool Argument::hasContent() const {
	return has_Content;
}

bool Argument::operator==(const Argument& arg) const noexcept {
	
	if (!arg.getArg1().empty()) {
		if (arg.getArg1() == getArg1() || arg.getArg1() == getArg2())
			return true;
	}

	if (!arg.getArg2().empty()) {
		if (arg.getArg2() == getArg1() || arg.getArg2() == getArg2())
			return true;
	}

	return false;
}

bool Argument::operator==(const std::string& arg) const noexcept {
	return (arg == getArg1() || arg == getArg2());
}
