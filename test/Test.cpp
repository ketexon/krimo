#include "Test.hpp"

using namespace krimo::test::impl;

krimo::test::impl::ConsoleAttributeContext::ConsoleAttributeContext(Output output, Color fg, Color bg) : output{output} {
	PushConsoleColor(output, fg, bg);
}

krimo::test::impl::ConsoleAttributeContext::~ConsoleAttributeContext(){
	PopConsoleColor(output);
}

static std::string testCase = "";
void krimo::test::SetTestCase(std::string s){
	testCase = s;
}

std::string krimo::test::GetTestCase(){
	return testCase;
}

void krimo::test::Assert(bool cond, std::string msg, std::string conditionString, std::string file, int lineNumber) {
	if(!cond){
		if(testCase.length() > 0){
			auto ctx = ConsoleAttributeContext(Output::Stderr, Color::Red);
			std::cerr << "Test case \"" << testCase << "\" failed.\n";
		}

		std::cerr << file << " (" << lineNumber << "):";

		std::cerr << "\n\t" << conditionString;

		if(msg.length() > 0){
			{
				auto ctx = ConsoleAttributeContext(Output::Stderr, Color::Red);
				std::cerr << "\nMessage: ";
			}
			std::cerr << msg;
		}

		std::cerr << std::endl;

		std::exit(1);
	}
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stack>

HANDLE GetWindowsHandle(Output output) {
	return GetStdHandle(
		output == Output::Stderr
			? STD_ERROR_HANDLE
			: STD_OUTPUT_HANDLE
	);
}

static std::stack<WORD> wStderrAttributeStack;
static std::stack<WORD> wStdoutAttributeStack;

void krimo::test::impl::PushConsoleColor(Output output, Color fg, Color bg){
	std::stack<WORD>& stack = output == Output::Stderr ? wStderrAttributeStack : wStdoutAttributeStack;
	HANDLE handle = GetWindowsHandle(output);

	CONSOLE_SCREEN_BUFFER_INFO sbi;
	GetConsoleScreenBufferInfo(handle, &sbi);
	stack.push(sbi.wAttributes);

	WORD newAttr = 0;
	newAttr |= (static_cast<int>(fg) & static_cast<int>(Color::Red)		) ? FOREGROUND_RED 		: 0;
	newAttr |= (static_cast<int>(fg) & static_cast<int>(Color::Green)	) ? FOREGROUND_GREEN 	: 0;
	newAttr |= (static_cast<int>(fg) & static_cast<int>(Color::Blue)	) ? FOREGROUND_BLUE 	: 0;
	newAttr |= (static_cast<int>(bg) & static_cast<int>(Color::Red)		) ? BACKGROUND_RED 		: 0;
	newAttr |= (static_cast<int>(bg) & static_cast<int>(Color::Green)	) ? BACKGROUND_GREEN 	: 0;
	newAttr |= (static_cast<int>(bg) & static_cast<int>(Color::Blue)	) ? BACKGROUND_BLUE 	: 0;

	SetConsoleTextAttribute(handle, newAttr);
}

void krimo::test::impl::PopConsoleColor(Output output){
	std::stack<WORD>& stack = output == Output::Stderr ? wStderrAttributeStack : wStdoutAttributeStack;
	if(stack.size() > 0){
		WORD attr = stack.top();
		stack.pop();
		SetConsoleTextAttribute(GetWindowsHandle(output), attr);
	}
}
#else
// Unknown OS, don't do anything
void PushConsoleColor(Color color){

}

void PopConsoleColor(){

}
#endif