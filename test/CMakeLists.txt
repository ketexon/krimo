add_executable(
	Krimo_Simple_Test
	simple/simple.cpp
)

target_link_libraries(Krimo_Simple_Test Krimo)

set_target_properties(
	Krimo_Simple_Test
	PROPERTIES
	CXX_STANDARD 20
)