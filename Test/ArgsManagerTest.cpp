#include "CppUnitTest.h"
#include "../Source/ArgsManager.h"
#include "Auxiliary.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{		
	TEST_CLASS(ArgsManagerTest)
	{

	private:

		ArgsManager& argsManager = ArgsManager::getInstance();
	
	public:

		TEST_METHOD(argumentCheckOperators) {
			
			Argument argument1(false, "-a", "-b");
			Argument argument2(false, "-a");

			Argument argument3(false, "-c", "-a");

			Argument argument4(false, "-c");

			Assert::IsTrue(argument1 == argument2);
			Assert::IsTrue(argument2 == argument1);
			Assert::IsTrue(argument2 == argument3);
			Assert::IsTrue(argument3 == argument2);

			Assert::IsTrue(argument3 == argument4);
			Assert::IsTrue(argument4 == argument3);
			Assert::IsFalse(argument4 == argument2);

			Argument argument5 = argument1;
			Assert::IsTrue(argument5 == argument1);

			Argument argument6(argument1);
			Assert::IsTrue(argument6 == argument1);

			Argument argument7 = std::move(argument1);
			Assert::IsFalse(argument7 == argument1);

			Argument argument8(std::move(argument2));
			Assert::IsFalse(argument8 == argument2);

		}

		TEST_METHOD(checkDiplicationRequiredArgsAdd_1) {
			argsManager.clear();

			try {
				argsManager.addRequired(Argument(false, "-a", "--append"));
				argsManager.addRequired(Argument(false, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationRequiredArgsAdd_2) {
			argsManager.clear();


			try {
				argsManager.addRequired(Argument(false, "-a", "--append"));
				argsManager.addRequired(Argument(false, "--append"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationRequiredArgsAdd_3) {
			argsManager.clear();


			try {
				argsManager.addRequired(Argument(false, "-a", "--append"));
				argsManager.addRequired(Argument(true, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationRequiredArgsAdd_4) {
			argsManager.clear();


			try {
				argsManager.addRequired(Argument(true, "-a", "--append"));
				argsManager.addRequired(Argument(false, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationRequiredToSetArgsAdd_1) {
			argsManager.clear();


			try {
				argsManager.addRequiredToSet(Argument(false, "-a", "--append"));
				argsManager.addRequiredToSet(Argument(false, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationRequiredToSetArgsAdd_2) {
			argsManager.clear();


			try {
				argsManager.addRequiredToSet(Argument(false, "-a", "--append"));
				argsManager.addRequiredToSet(Argument(false, "--append"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationRequiredToSetArgsAdd_3) {
			argsManager.clear();


			try {
				argsManager.addRequiredToSet(Argument(false, "-a", "--append"));
				argsManager.addRequiredToSet(Argument(true, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationRequiredToSetArgsAdd_4) {
			argsManager.clear();


			try {
				argsManager.addRequiredToSet(Argument(true, "-a", "--append"));
				argsManager.addRequiredToSet(Argument(false, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationOptionalArgsAdd_1) {
			argsManager.clear();


			try {
				argsManager.addOptional(Argument(false, "-a", "--append"));
				argsManager.addOptional(Argument(false, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationOptionalArgsAdd_2) {
			argsManager.clear();


			try {
				argsManager.addOptional(Argument(false, "-a", "--append"));
				argsManager.addOptional(Argument(false, "--append"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationOptionalArgsAdd_3) {
			argsManager.clear();


			try {
				argsManager.addOptional(Argument(false, "-a", "--append"));
				argsManager.addOptional(Argument(true, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(checkDiplicationOptionalArgsAdd_4) {
			argsManager.clear();


			try {
				argsManager.addOptional(Argument(true, "-a", "--append"));
				argsManager.addOptional(Argument(false, "-a"));
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Parameter already added but no exception thrown");
		}

		TEST_METHOD(requiredArg_valid_1) {
				argsManager.clear();

				const unsigned int argc = 1;
				const char* argv[] = {
					"-a"
				};

				argsManager.addRequired(Argument(false, "-a"));

				try {
					argsManager.parse(argc, argv, 0);
					Assert::IsTrue(argsManager.argPresent("-a"));
				}
				catch (const std::exception& ex) {
					Assert::Fail(toWstring(ex.what()).c_str());
				}
			}

		TEST_METHOD(requiredArg_valid_2) {
			argsManager.clear();

			const unsigned int argc = 1;

			const char* argv[] = {
				"-a"
			};

			argsManager.addRequired(Argument(false, "-a", "--append"));

			try {
				argsManager.parse(argc, argv, 0);
				Assert::IsTrue(argsManager.argPresent("-a"));
				Assert::IsTrue(argsManager.argPresent("--append"));
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}
		
		TEST_METHOD(requiredArg_invalid_1) {
			argsManager.clear();

			const unsigned int argc = 1;
			const char* argv[] = {
				"-b"
			};

			argsManager.addRequired(Argument(false, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required argument not set, but no exception was thrown!");
		}

		TEST_METHOD(requiredArgWithContent_valid_1) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			argsManager.addRequired(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
				Assert::IsTrue(argsManager.argValue("-a") == "helloWorld");
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}

		TEST_METHOD(requiredArgWithContent_invalid_1) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "-b"
			};

			argsManager.addRequired(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(requiredArgWithContent_invalid_2) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", ""
			};

			argsManager.addRequired(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(requiredArgWithContent_invalid_3) {
			argsManager.clear();

			const unsigned int argc = 1;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			argsManager.addRequired(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(requiredArgWithContent_invalid_4) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", nullptr
			};

			argsManager.addRequired(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(addRequiredArgToSet_valid) {
			argsManager.clear();

			const unsigned int argc = 3;
			const char* argv[] = {
				"-a", "--append", "-b"
			};

			argsManager
				.addRequiredToSet(Argument(false, "-a", "--append"))
				.addRequiredToSet(Argument(false, "-b"))
				.addRequiredToSet(Argument(false, "--test"));

			try {
				argsManager.parse(argc, argv, 0);
				Assert::IsTrue(argsManager.argPresent("-a"));
				Assert::IsTrue(argsManager.argPresent("--append"));
				Assert::IsTrue(argsManager.argPresent("-b"));
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}

		TEST_METHOD(addRequiredArgToSet_invalid) {
			argsManager.clear();

			const unsigned int argc = 1;

			const char* argv_1[] = {
				"-b"
			};

			argsManager
				.addRequiredToSet(Argument(false, "-a", "--append"));

			try {
				argsManager.parse(argc, argv_1, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required argument not set, but no exception was thrown!");
		}

		TEST_METHOD(addRequiredArgToSetWithContent_valid) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			argsManager.addRequiredToSet(Argument(true, "-a", "--append"));

			try {
				argsManager.parse(argc, argv, 0);
				Assert::IsTrue(argsManager.argValue("-a") == "helloWorld");
				Assert::IsTrue(argsManager.argValue("--append") == "helloWorld");
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}

		TEST_METHOD(addRequiredArgToSetWithContent_invalid_1) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "-b"
			};

			argsManager.addRequiredToSet(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(addRequiredArgToSetWithContent_invalid_2) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", ""
			};

			argsManager.addRequiredToSet(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(addRequiredArgToSetWithContent_invalid_3) {
			argsManager.clear();

			const unsigned int argc = 1;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			argsManager.addRequiredToSet(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(addRequiredArgToSetWithContent_invalid_4) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", nullptr
			};

			argsManager.addRequiredToSet(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(addRequiredArgToSetWithContent_invalid_5) {
			argsManager.clear();

			const unsigned int argc = 3;
			const char* argv[] = {
				"-a", "-b", ""
			};

			argsManager
				.addRequiredToSet(Argument(true,  "-a"))
				.addRequiredToSet(Argument(false, "-b"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(optionalArg) {
			argsManager.clear();

			const unsigned int argc = 1;
			const char* argv[] = {
				"-a"
			};

			argsManager.addOptional(Argument(false, "-a", "--append"));

			try {
				argsManager.parse(argc, argv, 0);

				Assert::IsTrue(argsManager.argPresent("-a"));
				Assert::IsTrue(argsManager.argPresent("--append"));
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}

		TEST_METHOD(optionalArg_empty) {
			argsManager.clear();

			const unsigned int argc = 0;

			argsManager.addOptional(Argument(false, "-q"));

			try {
				argsManager.parse(argc, nullptr, 0);
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}

		TEST_METHOD(optionalArgWithContent_empty) {
			argsManager.clear();

			const unsigned int argc = 0;

			argsManager.addOptional(Argument(true, "-q"));

			try {
				argsManager.parse(argc, nullptr, 0);
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}

		TEST_METHOD(optionalArgWithContent_valid) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			argsManager.addOptional(Argument(true,  "-a"));
			argsManager.addOptional(Argument(false, "-b"));

			try {
				argsManager.parse(argc, argv, 0);
				Assert::IsTrue(argsManager.argValue("-a") == "helloWorld");
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}

		TEST_METHOD(optionalArgWithContent_invalid_1) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "-b"
			};

			argsManager.addOptional(Argument(true,  "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(optionalArgWithContent_invalid_2) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", ""
			};

			argsManager.addOptional(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(optionalArgWithContent_invalid_3) {
			argsManager.clear();

			const unsigned int argc = 1;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			argsManager.addOptional(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(optionalArgWithContent_invalid_4) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", nullptr
			};

			argsManager.addOptional(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(optionalArgWithContent_invalid_5) {
			argsManager.clear();

			const unsigned int argc = 3;
			const char* argv[] = {
				"-a", "-b", ""
			};

			argsManager
				.addOptional(Argument(true, "-a"))
				.addOptional(Argument(false, "-b"));

			try {
				argsManager.parse(argc, argv, 0);
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Required content not set, but no exception was thrown!");
		}

		TEST_METHOD(optionalArgWithContent_emptyArgs) {
			argsManager.clear();

			const unsigned int argc = 0;
			const char* argv[] = {
				"-a"
			};

			argsManager.addOptional(Argument(false , "-a"));

			try {
				argsManager.parse(argc, argv, 0);			
			}
			catch (...) {
				Assert::Fail(L"An exception was thrown, but it shouldn't be!");
			}
		}
		
		TEST_METHOD(valueContent_valid) {
			argsManager.clear();
			
			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			const Argument arg = Argument(true, "-a");
			argsManager.addRequired(arg);

			try {
				argsManager.parse(argc, argv, 0);
				Assert::IsTrue(argsManager.argValue(arg)=="helloWorld");
			}
			catch (const std::exception& ex) {
				Assert::Fail(toWstring(ex.what()).c_str());
			}
		}

		TEST_METHOD(valueContent_invalid_1) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			argsManager.addRequired(Argument(true, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
				argsManager.argValue("-b");

			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Passed invalid Argument, but no exception was thrown!");
		}

		TEST_METHOD(valueContent_invalid_2) {
			argsManager.clear();

			const unsigned int argc = 2;
			const char* argv[] = {
				"-a", "helloWorld"
			};

			argsManager.addRequired(Argument(false, "-a"));

			try {
				argsManager.parse(argc, argv, 0);
				argsManager.argValue("-a");
			}
			catch (...) {
				return;
			}

			Assert::Fail(L"Passed Argument without content (invalid argument), but no exception was thrown!");
		}

		TEST_METHOD(checkHelp) {
			argsManager.clear();

			argsManager
				.addHelp("-h")
				.addHelp("--help");

			int argc = 1;

			const char* argv[] = {
				"-h"
			};

			Assert::IsTrue(argsManager.isHelpArg(argc, argv, 0));
		}

	};

}