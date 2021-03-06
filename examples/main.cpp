/** 
 * The example demonstrates the use of the library for the file copy/move utility.
 */

#include "../Source/ArgsManager.h"
#include <filesystem>

static const Argument& inputParam = Argument(true,  "-i", "--input");
static const Argument& outputParam = Argument(true, "-o", "--output");

static const Argument& moveParam = Argument(false, "--move");
static const Argument& copyParam = Argument(false, "--copy");

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
		argsManager
			.addHelp("-h")
			.addHelp("--h")
			.addHelp("--help")
			.addHelp("help")
			.addHelp("/?");

		// Add required parameters
		argsManager
			.addRequired(inputParam)
			.addRequired(outputParam);

		argsManager
			.addRequiredToSet(moveParam)
			.addRequiredToSet(copyParam);

		// Check help
		if (argsManager.isHelpArg(argc, argv, 1)) {
			std::cout << usageMsg << std::endl;
			return 0;
		}

		// Parse
		argsManager.parse(argc, argv, 1);

		// Get content
		const Content& inputFile = argsManager.argValue(inputParam);   // may be "-i" or "--input"
		const Content& outputFile = argsManager.argValue(outputParam); // may be "-o" or "--output"

		// Move or copy

		// Move (takes predence in this case)
		if (argsManager.argPresent(moveParam)) {
			std::filesystem::rename(inputFile, outputFile);
			return 0;
		}

		// Copy
		if (argsManager.argPresent(copyParam))
			std::filesystem::copy(inputFile, outputFile);

		return 0;
	}
	catch (const InvalidArg& invalidArgException) {
		std::cerr << "Error: " << invalidArgException.what() << std::endl;
		std::cerr << usageMsg << std::endl;
		return 1;
	}
	catch (const std::exception& exception) {
		std::cerr << "Critical error: " << exception.what() << std::endl;
		return 1;
	}

	return 0;
}