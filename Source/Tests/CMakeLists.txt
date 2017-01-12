set(target DeliberationTests)

include_directories(
	${target}
	PRIVATE

	${DELIBERATION_INCLUDE_DIRS}
	${DELIBERATION_DIR}/Extern/googletest/googletest/include/
)

add_executable(
	${target}

	Tests.cpp
	PrimitiveIntersectionTest.cpp
)

target_link_libraries(${target}
	Deliberation
	gtest
	)