add_executable(
	Krimo_Simple_Test
	simple/Simple.cpp
	Test.hpp
)

add_executable(
	Krimo_Types_Test
	types/Types.cpp
	Test.hpp Test.cpp
)

target_link_libraries(Krimo_Simple_Test Krimo)
target_link_libraries(Krimo_Types_Test Krimo)

set_target_properties(
	Krimo_Simple_Test Krimo_Types_Test
	PROPERTIES
	CXX_STANDARD 20
)