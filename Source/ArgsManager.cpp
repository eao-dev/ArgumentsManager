#include "ArgsManager.h"

ArgContentMap argContentMap;

ArgContent ArgsManager::getContent(const Argument& arg,
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
	requiredArgs.insert(requredArg);
	return *this;
}

ArgsManager& ArgsManager::addOptional(const Argument& optionalParameter)
{
	this->optionalArgs.insert(optionalParameter);
	return *this;
}

ArgsManager& ArgsManager::addRequiredToSet(const Argument& arg)
{
	requiredArgSet.insert(arg);
	return *this;
}

void ArgsManager::clear()
{
	requiredArgs.clear();
	requiredArgSet.clear();
	optionalArgs.clear();
}

void ArgsManager::parse(const unsigned int argc, const char* const argv[], unsigned int beginIdx = 0)
{
	if (argc == 0 && (!requiredArgs.empty() || !requiredArgSet.empty()))
		throw InvalidArg("Does not pass a list of arguments.");

	if (beginIdx > argc)
		throw std::invalid_argument("Start index out of bounds.");

	if (argv == nullptr)
		throw std::invalid_argument("Pointer argv is NULL!");

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

				if (requiredParam == paramStr) {
					paramFound = true;

					ArgContent parameterContent;

					if (requiredParam.hasContent())
						parameterContent = getContent(requiredParam, argc, idx, argv);

					// Fill
					argContentMap.insert(std::pair<Argument, ArgContent>(requiredParam, parameterContent));
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

				if (requiredParam == paramStr) {
					paramFound = true;

					ArgContent parameterContent;

					if (requiredParam.hasContent())
						parameterContent = getContent(requiredParam, argc, idx, argv);

					// Fill
					argContentMap.insert(std::pair<Argument, ArgContent>(requiredParam, parameterContent));
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

				if (optionParam == paramStr) {

					ArgContent parameterContent;

					if (optionParam.hasContent())
						parameterContent = getContent(optionParam, argc, idx, argv);

					// Fill
					argContentMap.insert(std::pair<Argument, ArgContent>(optionParam, parameterContent));
					break;
				}
			}
		}
	}
}

ArgContent ArgsManager::argValue(const Argument& param) const
{
	if (!parsed)
		throw std::runtime_error("Parsing failed");

	const auto& paramContentIter = argContentMap.find(param);

	std::string exceptionMessage;
	const auto& p2 = param.getArg2();

	if (paramContentIter == argContentMap.end()) {
		exceptionMessage
			.append("Parameter '")
			.append(param.getArg1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
			.append(" not found!");
		throw InvalidArg(exceptionMessage);
	}

	if (!(*paramContentIter).first.hasContent()) {
		exceptionMessage
			.append("Parameter '")
			.append(param.getArg1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
			.append(" has no content!");
		throw std::runtime_error(exceptionMessage);
	}

	return (*paramContentIter).second;
}

bool ArgsManager::argPresent(const Argument& arg) const
{
	if (!parsed)
		throw std::runtime_error("Parsing failed");

	return argContentMap.find(arg)!= argContentMap.end();
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