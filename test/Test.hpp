#pragma once

#include <string>
#include <iostream>
#include <cstdlib>

namespace krimo {
	namespace test {
		namespace impl {
			enum Output {
				Stdout	= 0b00000001,
				Stderr	= 0b00000010,
			};
			enum class Color {
				Black		= 0b00000000,
				Red 		= 0b00000001,
				Green 		= 0b00000010,
				Blue 		= 0b00000100,
			};
			void PushConsoleColor(Output, Color fg, Color bg = Color::Black);
			void PopConsoleColor(Output);

			struct ConsoleAttributeContext {
				ConsoleAttributeContext(Output, Color fg, Color bg = Color::Black);
				~ConsoleAttributeContext();
			private:
				Output output;
			};
		}

		void Assert(bool cond, std::string msg = "", std::string conditionString = "", std::string file = "", int lineNumber = 0);

		inline void TestsPassed(){
			using namespace impl;

			auto ctx = ConsoleAttributeContext(Output::Stdout, Color::Green);
			std::cout << "All Tests Passed :)" << std::endl;
		}

		void SetTestCase(std::string name);
		std::string GetTestCase();
	}
}

#define KRIMO_ASSERT_1(cond) krimo::test::Assert(cond, "", #cond, __FILE__, __LINE__)
#define KRIMO_ASSERT_2(cond, msg) krimo::test::Assert(cond, msg, #cond, __FILE__, __LINE__)
#define KRIMO_3RD_ARG(arg1, arg2, arg3, ...) arg3
#define KRIMO_ASSERT_CHOOSER(...) KRIMO_3RD_ARG(__VA_ARGS__, KRIMO_ASSERT_2, KRIMO_ASSERT_1)
#define KRIMO_ASSERT(...) KRIMO_ASSERT_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define KRIMO_ASSERT_THROWS(cond) { try { cond; krimo::test::Assert(false, "Condition does not throw.", #cond, __FILE__, __LINE__); } catch(...) {} }
#define KRIMO_ASSERT_NOTHROW(cond) { try { cond; } catch(...) { krimo::test::Assert(false, "Condition throws.", #cond, __FILE__, __LINE__); } }

#define KRIMO_TESTS_PASSED() krimo::test::TestsPassed();

#define KRIMO_TEST_CASE(name) krimo::test::SetTestCase(name);