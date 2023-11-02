cmake_minimum_required(VERSION 3.25)

add_subdirectory(ext/glm)

option(Krimo_Build_Tests "Build Krimo tests" ON)

project(Krimo LANGUAGES CXX)

set(
	Sources
	Krimo.hpp Krimo.cpp
	EventSystem.hpp EventSystem.cpp
	RenderingSystem.hpp
	WindowSystem.hpp WindowSystem.cpp
	System.hpp System.cpp
	SceneTree.hpp SceneTree.cpp
	types/Delegate.hpp types/Delegate.cpp
	types/TypeID.hpp types/Property.hpp types/Math.hpp
	types/Object.hpp
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

target_link_libraries(
	Krimo
	PUBLIC glm
)

set_target_properties(
	Krimo
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

if(Krimo_Build_Tests)
	add_subdirectory(test)
endif()