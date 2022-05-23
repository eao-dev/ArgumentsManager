#include "ArgsManager.h"

ArgContent ArgsManager::getContentForArg(const Argument& arg,
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

void ArgsManager::clearAllParams()
{
	requiredArgs.clear();
	requiredArgSet.clear();
	optionalArgs.clear();
}

ArgContentMap ArgsManager::parse(const unsigned int argc, const char * const argv[], unsigned int beginIdx = 0) const
{
	// TODO: add support only optional arguments

	if (argc == 0)
		throw InvalidArg("Does not pass a list of arguments.");

	if (beginIdx > argc)
		throw std::invalid_argument("Start index out of bounds.");

	if (argv == nullptr)
		throw std::invalid_argument("Pointer argv is NULL!");

	std::string exceptionMessage;

	ArgContentMap 
		  requiredParamContentMap
		, requiredSetParamContentMap
		, optionalParamContentMap
		, out;

	// Required parameters
	for (const auto& requiredParam : requiredArgs) {
		bool paramFound = false;

		for (unsigned int idx = beginIdx; idx < argc; ++idx) {
			const char* const paramStr = argv[idx];

			if (paramStr == nullptr)
				throw std::runtime_error("Argument " + std::to_string(idx + 1) + " is NULL");
			
			if (requiredParam == paramStr) {

				ArgContent parameterContent;

				if (requiredParam.hasContent()) 
					parameterContent = getContentForArg(requiredParam, argc, idx, argv);

				requiredParamContentMap.insert(std::pair<Argument, ArgContent>(requiredParam, parameterContent));
				paramFound = true;
				break;
			}
		}

		if (!paramFound) {
			const auto& p2 = requiredParam.getArg2();
			exceptionMessage
				.append("Parameter '")
				.append(requiredParam.getArg1() + ((!p2.empty()) ? "' / '" + p2 + "'":"'"))
				.append(" not found!");
			goto NOT_FOUND_EXCEPTION;
		}
	}

	// Required parameters set
	if (!requiredArgSet.empty()) {
		for (unsigned int idx = beginIdx; idx < argc; ++idx) {

			const char* const paramStr = argv[idx];
			if (paramStr == nullptr)
				throw std::runtime_error("Parameter " + std::to_string(idx + 1) + " is NULL");

			for (const auto& requiredParam : requiredArgSet) {
				if (requiredParam == paramStr) {
					ArgContent parameterContent;
					if (requiredParam.hasContent())
						parameterContent = getContentForArg(requiredParam, argc, idx, argv);

					requiredSetParamContentMap.insert(std::pair<Argument, ArgContent>(requiredParam, parameterContent));
					break;
				}
			}
		}
	}


	if (!requiredArgSet.empty() && requiredSetParamContentMap.empty()) {
		exceptionMessage = "Required argument not found.";
		goto NOT_FOUND_EXCEPTION;
	}

	// Optional parameters set
	if (!optionalArgs.empty()) {
		for (unsigned int idx = beginIdx; idx < argc; ++idx) {

			const char* const paramStr = argv[idx];
			if (paramStr == nullptr)
				throw std::runtime_error("Argument " + std::to_string(idx + 1) + " is NULL");

			for (const auto& optionParam : optionalArgs) {
				if (optionParam == paramStr) {
					ArgContent parameterContent;
					if (optionParam.hasContent())
						parameterContent = getContentForArg(optionParam, argc, idx, argv);

					optionalParamContentMap.insert(std::pair<Argument, ArgContent>(optionParam, parameterContent));
					break;
				}
			}
		}
	}


	// Fill
	out.insert(requiredParamContentMap.begin(), requiredParamContentMap.end());
	out.insert(requiredSetParamContentMap.begin(), requiredSetParamContentMap.end());
	out.insert(optionalParamContentMap.begin(), optionalParamContentMap.end());
	
	return out;

NOT_FOUND_EXCEPTION:
	throw InvalidArg(exceptionMessage);
}

ArgContent ArgsManager::argValue(const ArgContentMap & paramContentMap, const Argument & param) const
{
	const auto& paramContentIter = paramContentMap.find(param);
	
	std::string exceptionMessage;
	const auto& p2 = param.getArg2();

	if (paramContentIter == paramContentMap.end()) {
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

bool ArgsManager::isHelpArg(const unsigned int argc, const char * const argv[], unsigned int beginIdx) const
{
	if (argc == 0)
		throw InvalidArg("Does not pass a list of arguments.");

	if (beginIdx > (argc))
		throw std::invalid_argument("Start index out of bounds.");

	if (argv == nullptr)
		throw std::invalid_argument("Pointer argv is NULL!");

	for (unsigned int idx = beginIdx; idx < argc; ++idx) {
		for (const auto& helpParamItem : helpArgs) {
			if (helpParamItem == argv[idx])
				return true;
		}
	}

	return false;
}