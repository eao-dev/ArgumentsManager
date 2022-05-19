/*
*
 * The example demonstrates the use of the library for the file copy/move utility.
 *
 */

#include "ArgsManager.h"
#include <filesystem>

static const Parameter& inputParam = Parameter(true,  "-i", "--input");
static const Parameter& outputParam = Parameter(true, "-o", "--output");

static const Parameter& moveParam = Parameter(false, "--move");
static const Parameter& copyParam = Parameter(false, "--copy");

std::unordered_set<std::string> helpParams = {
	"-h",
	"--h",
	"--help",
	"help",
	"/?"
};

static const char* const usageMsg = 
"Usage: app -i <file> -o <file> [--copy] | [--move] \n"
"\n -i or --input   input file"
"\n -o or --output  output file"
"\n --move          move file"
"\n --copy          copy file";

int main(int argc, char* argv[])
{
	ArgsManager& argsManager = ArgsManager::getInstance();

	try {
		// Add help parameters
		argsManager.addHelpParam(helpParams);

		// Add required parameters
		argsManager.addRequiredParam(inputParam);
		argsManager.addRequiredParam(outputParam);

		argsManager.setRequiredParamSet({
			moveParam,
			copyParam
		});

		// Check help
		if (argsManager.isHelpParam(argc, argv, 1)) {
			std::cout << usageMsg << std::endl;
			return 0;
		}

		// Parse
		const ParamContentMap& paramContentMap = argsManager.parse(argc, argv, 1);

		// Get content
		const ParameterContent& inputFile = argsManager.paramValue(paramContentMap, inputParam);
		const ParameterContent& outputFile = argsManager.paramValue(paramContentMap, outputParam);

		// Move or copy

		// Move (takes predence in this case)
		if (paramContentMap.find(moveParam) != paramContentMap.end()) {
			std::filesystem::rename(inputFile, outputFile);
			return 0;
		}

		// Copy
		if (paramContentMap.find(copyParam) != paramContentMap.end())
			std::filesystem::copy(inputFile, outputFile);

		return 0;
	}
	catch (const InvalidParameter& invalidParameterException) {
		std::cerr << "Error: " << invalidParameterException.what() << std::endl;
		std::cerr << usageMsg << std::endl;
		return 1;
	}
	catch (const std::exception& exception) {
		std::cerr << "Critical error: " << exception.what() << std::endl;
		return 1;
	}

	return 0;
}