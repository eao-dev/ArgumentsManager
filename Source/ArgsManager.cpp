#include "ArgsManager.h"

Content ArgsManager::getContent(const Argument& arg,
	const unsigned int argc, const unsigned int idx, const char* const argv[]) const
{
	const char* const nextParam = argv[idx + 1];

	// Extract content
	if ((idx == (argc - 1)) || nextParam == nullptr || nextParam[0] == '-' || nextParam[0] == '\0') {
		const auto& p2 = arg.getArg2();
		std::string exceptionMessage;

		exceptionMessage
			.append("Argument '")
			.append(arg.getArg1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
			.append(" not found!");

		throw InvalidArg(exceptionMessage);
	}
	return nextParam;
}

bool ArgsManager::checkExists(const Argument& argument)
{
	for (const auto& arg : requiredArgs) {
		if (argument == arg)
			return true;
	}

	for (const auto& arg : requiredArgSet) {
		if (argument == arg)
			return true;
	}

	for (const auto& arg : optionalArgs) {
		if (argument == arg)
			return true;
	}

	return false;
}

ArgsManager& ArgsManager::getInstance()
{
	static ArgsManager argsManager;
	return argsManager;
}

ArgsManager::~ArgsManager()
{
}

ArgsManager& ArgsManager::addHelp(const std::string& helpParam)
{
	helpArgs.insert(helpParam);
	return *this;
}

ArgsManager& ArgsManager::addRequired(const Argument& requredArg)
{
	if (checkExists(requredArg))
		throw std::runtime_error("This argument has already been added");
	requiredArgs.push_back(requredArg);
	return *this;
}

ArgsManager& ArgsManager::addOptional(const Argument& optionalArg)
{
	if (checkExists(optionalArg))
		throw std::runtime_error("This argument has already been added");
	optionalArgs.push_back(optionalArg);
	return *this;
}

ArgsManager& ArgsManager::addRequiredToSet(const Argument& arg)
{
	if (checkExists(arg))
		throw std::runtime_error("This argument has already been added");
	requiredArgSet.push_back(arg);
	return *this;
}

void ArgsManager::clear()
{
	requiredArgs.clear();
	requiredArgSet.clear();
	optionalArgs.clear();
	argContentList.clear();
}

void ArgsManager::parse(const unsigned int argc, const char* const argv[], unsigned int beginIdx = 0)
{
	if (argc == 0 && (!requiredArgs.empty() || !requiredArgSet.empty())) {
		throw InvalidArg("Does not pass a list of arguments.");
	}

	if (argc > 0) {
		if (argv == nullptr)
			throw std::invalid_argument("Pointer argv is NULL!");
	}

	if (beginIdx > argc)
		throw std::invalid_argument("Start index out of bounds.");

	std::string exceptionMessage;
	parsed = true;

	// Required arguments
	if (!requiredArgs.empty()) {

		for (const auto& requiredParam : requiredArgs) {
			bool paramFound = false;

			for (unsigned int idx = beginIdx; idx < argc; ++idx) {
				const char* const paramStr = argv[idx];

				if (paramStr == nullptr)
					throw std::runtime_error("Argument " + std::to_string(idx + 1) + " is NULL");

				if (requiredParam == Argument(paramStr)) {
					paramFound = true;

					Content content;

					if (requiredParam.hasContent())
						content = getContent(requiredParam, argc, idx, argv);

					// Fill
					argContentList.push_back({ requiredParam, content });
					break;
				}
			}

			if (!paramFound) {
				const auto& p2 = requiredParam.getArg2();
				exceptionMessage
					.append("Parameter '")
					.append(requiredParam.getArg1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
					.append(" not found!");
				throw InvalidArg(exceptionMessage);
			}
		}
	}

	// Required arguments set
	if (!requiredArgSet.empty()) {
		bool paramFound = false;

		for (const auto& requiredParam : requiredArgSet) {

			for (unsigned int idx = beginIdx; idx < argc; ++idx) {
				const char* const paramStr = argv[idx];

				if (paramStr == nullptr)
					throw std::runtime_error("Parameter " + std::to_string(idx + 1) + " is NULL");

				if (requiredParam == Argument(paramStr)) {
					paramFound = true;

					Content content;

					if (requiredParam.hasContent())
						content = getContent(requiredParam, argc, idx, argv);

					// Fill
					argContentList.push_back({ requiredParam, content });
					break;
				}
			}
		}

		if (!paramFound) {
			exceptionMessage = "Required argument not found.";
			throw InvalidArg(exceptionMessage);
		}

	}

	// Optional arguments
	if (!optionalArgs.empty()) {

		for (const auto& optionParam : optionalArgs) {

			for (unsigned int idx = beginIdx; idx < argc; ++idx) {
				const char* const paramStr = argv[idx];

				if (paramStr == nullptr)
					throw std::runtime_error("Argument " + std::to_string(idx + 1) + " is NULL");

				if (optionParam == Argument(paramStr)) {

					Content content;

					if (optionParam.hasContent())
						content = getContent(optionParam, argc, idx, argv);

					// Fill
					argContentList.push_back({ optionParam, content });
					break;
				}
			}
		}
	}
}

Content ArgsManager::argValue(const Argument& arg) const
{
	if (!parsed)
		throw std::runtime_error("Parsing failed");

	const ArgContent* argContent=nullptr;
	for (const auto& it : argContentList) {
		if (it.arg == arg)
			argContent = &it;
	}

	std::string exceptionMessage;
	const auto& p2 = arg.getArg2();

	if (argContent == nullptr) {
		exceptionMessage
			.append("Parameter '")
			.append(arg.getArg1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
			.append(" not found!");
		throw InvalidArg(exceptionMessage);
	}

	if (!argContent->arg.hasContent()) {
		exceptionMessage
			.append("Parameter '")
			.append(arg.getArg1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
			.append(" has no content!");
		throw std::runtime_error(exceptionMessage);
	}

	return argContent->content;
}

bool ArgsManager::argPresent(const Argument& arg) const
{
	if (!parsed)
		throw std::runtime_error("Parsing failed");

	for (const auto& argItem : argContentList) {
		if (argItem.arg == arg)
			return true;
	}
	return false;
}

bool ArgsManager::isHelpArg(const unsigned int argc, const char* const argv[], unsigned int beginIdx) const
{
	if (argc == 0)
		return false;

	if (beginIdx > argc)
		throw std::invalid_argument("Start index out of bounds.");

	if (argv == nullptr)
		throw std::invalid_argument("Pointer argv is NULL!");

	for (const auto& helpArgItem : helpArgs) {
		for (unsigned int idx = beginIdx; idx < argc; ++idx) {
			if (helpArgItem == argv[idx])
				return true;
		}
	}

	return false;
}