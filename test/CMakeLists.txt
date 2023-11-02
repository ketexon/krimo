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

add_executable(
	Krimo_Hash_Bench
	bench/HashBench.cpp
)

set(CMAKE_CXX_STANDARD 20)

target_link_libraries(Krimo_Simple_Test Krimo)
target_link_libraries(Krimo_Types_Test Krimo)
target_link_libraries(Krimo_Hash_Bench Krimo)

set_target_properties(
	Krimo_Simple_Test Krimo_Types_Test Krimo_Hash_Bench
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED TRUE
)