#include "ArgsManager.h"

ParameterContent ArgsManager::getContentForParam(const Parameter& parameter,
	const unsigned int argc, const unsigned int idx, const char* const argv[]) const
{
	const char* const nextParam = argv[idx + 1];

	// Extract content
	if ((idx == (argc - 1)) || nextParam == nullptr || nextParam[0] == '-' || nextParam[0] == '\0') {
		const auto& p2 = parameter.getParamName2();
		std::string exceptionMessage;
			
		exceptionMessage
			.append("Parameter '")
			.append(parameter.getParamName1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
			.append(" not found!");

		throw InvalidParameter(exceptionMessage);
	}
	return nextParam;
}

/*
 * Returns instance of this class.
 *
 */
ArgsManager& ArgsManager::getInstance()
{
	static ArgsManager argsManager;
	return argsManager;
}

ArgsManager::~ArgsManager()
{
}

void ArgsManager::addHelpParam(const std::unordered_set<std::string>& helpParams)
{
	this->helpParams = helpParams;
}

void ArgsManager::addRequiredParam(const Parameter& requredParameter)
{
	requiredParams.insert(requredParameter);
}

void ArgsManager::addOptionalParam(const Parameter& optionalParameter)
{
	this->optionalParameter.insert(optionalParameter);
}

void ArgsManager::setRequiredParamSet(const std::unordered_set<Parameter>& paramSet)
{
	requiredParamSet.insert(paramSet.begin(), paramSet.end());
}

void ArgsManager::setDepenentParams(const Parameter & requredParameter, const std::unordered_set<Parameter>& paramSet)
{
	// TODO: implement this method
	throw std::runtime_error("Method (setDepenentParams) not implemented");
}

void ArgsManager::clearAllParams()
{
	requiredParams.clear();
	requiredParamSet.clear();
	optionalParameter.clear();
}

ParamContentMap ArgsManager::parse(const unsigned int argc, const char * const argv[], unsigned int beginIdx = 0) const
{
	// TODO: added support only optional arguments
	// TODO: added the ability to display a compile list of missing arguments

	if (argc == 0)
		throw InvalidParameter("Does not pass a list of arguments.");

	if (beginIdx > (argc))
		throw std::invalid_argument("Start index out of bounds.");

	if (argv == nullptr)
		throw std::invalid_argument("Pointer argv is NULL!");

	std::string exceptionMessage;

	ParamContentMap 
		  requiredParamContentMap
		, requiredSetParamContentMap
		, optionalParamContentMap
		, out;

	// Required parameters
	for (const auto& requiredParam : requiredParams) {
		bool paramFound = false;

		for (unsigned int idx = beginIdx; idx < argc; ++idx) {
			const char* const paramStr = argv[idx];

			if (paramStr == nullptr)
				throw std::runtime_error("Parameter " + std::to_string(idx + 1) + " is NULL");
			
			if (requiredParam == paramStr) {

				ParameterContent parameterContent;

				if (requiredParam.hasContent()) 
					parameterContent = getContentForParam(requiredParam, argc, idx, argv);

				requiredParamContentMap.insert(std::pair<Parameter, ParameterContent>(requiredParam, parameterContent));
				paramFound = true;
				break;
			}
		}

		if (!paramFound) {
			const auto& p2 = requiredParam.getParamName2();
			exceptionMessage
				.append("Parameter '")
				.append(requiredParam.getParamName1() + ((!p2.empty()) ? "' / '" + p2 + "'":"'"))
				.append(" not found!");
			goto NOT_FOUND_EXCEPTION;
		}
	}

	// Required parameters set
	if (!requiredParamSet.empty()) {
		for (unsigned int idx = beginIdx; idx < argc; ++idx) {

			const char* const paramStr = argv[idx];
			if (paramStr == nullptr)
				throw std::runtime_error("Parameter " + std::to_string(idx + 1) + " is NULL");

			for (const auto& requiredParam : requiredParamSet) {
				if (requiredParam == paramStr) {
					ParameterContent parameterContent;
					if (requiredParam.hasContent())
						parameterContent = getContentForParam(requiredParam, argc, idx, argv);

					requiredSetParamContentMap.insert(std::pair<Parameter, ParameterContent>(requiredParam, parameterContent));
					break;
				}
			}
		}
	}


	if (!requiredParamSet.empty() && requiredSetParamContentMap.empty()) {
		exceptionMessage = "Required paramter not found.";
		goto NOT_FOUND_EXCEPTION;
	}

	// Optional parameters set
	if (!optionalParameter.empty()) {
		for (unsigned int idx = beginIdx; idx < argc; ++idx) {

			const char* const paramStr = argv[idx];
			if (paramStr == nullptr)
				throw std::runtime_error("Parameter " + std::to_string(idx + 1) + " is NULL");

			for (const auto& optionParam : optionalParameter) {
				if (optionParam == paramStr) {
					ParameterContent parameterContent;
					if (optionParam.hasContent())
						parameterContent = getContentForParam(optionParam, argc, idx, argv);

					optionalParamContentMap.insert(std::pair<Parameter, ParameterContent>(optionParam, parameterContent));
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
	throw InvalidParameter(exceptionMessage);
}

ParameterContent ArgsManager::paramValue(const ParamContentMap & paramContentMap, const Parameter & param) const
{
	const auto& paramContentIter = paramContentMap.find(param);
	
	std::string exceptionMessage;
	const auto& p2 = param.getParamName2();
	if (paramContentIter == paramContentMap.end()) {
		exceptionMessage
			.append("Parameter '")
			.append(param.getParamName1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
			.append(" not found!");
		throw InvalidParameter(exceptionMessage);
	}

	if (!(*paramContentIter).first.hasContent()) {
		exceptionMessage
			.append("Parameter '")
			.append(param.getParamName1() + ((!p2.empty()) ? "' / '" + p2 + "'" : "'"))
			.append(" has no content!");
		throw std::runtime_error(exceptionMessage);
	}
	

	return (*paramContentIter).second;
}

bool ArgsManager::isHelpParam(const unsigned int argc, const char * const argv[], unsigned int beginIdx) const
{
	if (argc == 0)
		throw InvalidParameter("Does not pass a list of arguments.");

	if (beginIdx > (argc))
		throw std::invalid_argument("Start index out of bounds.");

	if (argv == nullptr)
		throw std::invalid_argument("Pointer argv is NULL!");

	for (unsigned int idx = beginIdx; idx < argc; ++idx) {
		for (const auto& helpParamItem : helpParams) {
			if (helpParamItem == argv[idx])
				return true;
		}
	}

	return false;
}
