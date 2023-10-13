cmake_minimum_required(VERSION 3.25)

option(Krimo_Build_Tests "Build Krimo tests" ON)

project(Krimo)

set(
	Sources
	krimo.hpp krimo.cpp
)

list(TRANSFORM Sources PREPEND include/krimo/ REGEX .*\.hpp)
list(TRANSFORM Sources PREPEND src/ REGEX .*\.cpp)

add_library(
	Krimo
	${Sources}
)

target_include_directories(
	Krimo
	PUBLIC include
)

set_target_properties(
	Krimo
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED True
)

if(Krimo_Build_Tests)
	add_subdirectory(test)
endif()