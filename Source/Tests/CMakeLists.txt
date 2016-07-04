message(STATUS "Tests")

# target
set(target BanditTest)

# packages
find_package(Bandit     REQUIRED)
find_package(GLFW       REQUIRED)
find_package(glbinding  REQUIRED)
find_package(GLM        REQUIRED)

if (NOT BANDIT_FOUND)
	return()
endif()

# includes
include_directories(
    BEFORE

    ${PROJECT_SOURCE_DIR}/Source/Deliberation/include
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${GLBINDING_INCLUDE_DIR}
    ${BANDIT_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

# sources
set(sources
    BanditTest.cpp
    main.cpp
)

# libs
set(libs
    Deliberation
    ${GLFW_LIBRARY}
    ${GLBINDING_LIBRARIES}
)

# executable
add_executable(${target} ${sources})
target_link_libraries(${target} ${libs})
