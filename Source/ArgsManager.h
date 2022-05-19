#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "InvalidParameter.h"

static constexpr const unsigned int maxSavedInfo = 4;

// TODO: add move sem. constructor
struct Parameter {

private:

	const std::string paramName1;
	const std::string paramName2;
	static constexpr const char* emptyParamErrorMsg = "Add parameter cannot be empty!";
	bool has_Content = false;

public:

	explicit Parameter(const std::string& paramName1, const std::string& paramName2 = std::string()) :
		paramName1(paramName1), paramName2(paramName2) {
		if (paramName1.empty())
			throw std::invalid_argument(emptyParamErrorMsg);
	}

	explicit Parameter(bool hasContent, const std::string& paramName1, const std::string& paramName2 = std::string()):
		paramName1(paramName1), paramName2(paramName2), has_Content(hasContent) {
		if (paramName1.empty())
			throw std::invalid_argument(emptyParamErrorMsg);
	}

	explicit Parameter(bool hasContent, std::string&& paramName1, std::string&& paramName2):
		paramName1(std::move(paramName1)), paramName2(std::move(paramName2)), has_Content(hasContent) {}

	Parameter(const Parameter&) = default;
	Parameter& operator=(const Parameter&) = default;

	const std::string& getParamName1() const {
		return paramName1;
	};

	const std::string& getParamName2() const {
		return paramName2;
	};

	bool hasContent() const {
		return has_Content;
	}

	bool operator==(const Parameter& eqParam) const noexcept {
		return (getParamName1() == eqParam.getParamName1() && getParamName2() == eqParam.getParamName2());
	}

	bool operator==(const std::string& paramName) const noexcept {
		return (getParamName1() == paramName || getParamName2() == paramName);
	}
};

// Hash implementation for Parameter
namespace std {
	template<> 
	struct hash<Parameter> {
		std::size_t operator()(const Parameter& value) const noexcept {
			hash<string> hash;
			const std::size_t hashValue1 = hash(value.getParamName1());
			const std::size_t hashValue2 = hash(value.getParamName2());
			return hashValue1 ^ (hashValue2 << 1);
		}
	};
}

class ArgsManager;

using ParameterContent = std::string;
using ParamContentMap = std::unordered_map<Parameter, ParameterContent>;

class ArgsManager
{

private:

	ArgsManager() = default;	
	std::unordered_set<Parameter>
		  requiredParams
		, optionalParameter
		, requiredParamSet;

	ParameterContent getContentForParam(const Parameter& parameter, 
		const unsigned int argc, const unsigned int idx, const char* const argv[]) const;

	std::unordered_set<std::string> helpParams;

public:
	ArgsManager(const ArgsManager&) = delete;
	ArgsManager(const ArgsManager&&) = delete;
	void operator=(const ArgsManager&) = delete;
	void operator=(const ArgsManager&&) = delete;
	static ArgsManager& getInstance();
	virtual ~ArgsManager();

	void addHelpParam(const std::unordered_set<std::string>& helpParams);
	void addRequiredParam(const Parameter& requredParameter);
	void addOptionalParam(const Parameter& optionalParameter);
	void setRequiredParamSet(const std::unordered_set<Parameter>& paramSet);
	void setDepenentParams(const Parameter& requredParameter, const std::unordered_set<Parameter>& paramSet);
	void clearAllParams();

	ParamContentMap parse(const unsigned int argc, const char* const argv[], unsigned int beginIdx) const;
	ParameterContent paramValue(const ParamContentMap& paramContentMap, const Parameter& param) const;
	bool isHelpParam(const unsigned int argc, const char* const argv[], unsigned int beginIdx) const;
};