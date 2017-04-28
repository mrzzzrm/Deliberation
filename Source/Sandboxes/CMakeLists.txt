message(STATUS "Examples")

# includes
include_directories(
    BEFORE

    ${PROJECT_SOURCE_DIR}/Source/Deliberation/include
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${CXXOPTS_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIR}
    ${BANDIT_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

# libs
set(libs
    ${DELIBERATION_LIBRARIES}
)

# DrawSandbox
add_executable(DrawSandbox DrawSandbox.cpp)
target_link_libraries(DrawSandbox ${libs})

# ECSSandbox
add_executable(ECSSandbox ECSSandbox.cpp)
target_link_libraries(ECSSandbox ${libs})

# CoreSandbox
add_executable(CoreSandbox CoreSandbox.cpp)
target_link_libraries(CoreSandbox ${libs})

# SceneSandbox
add_executable(SceneSandbox SceneSandbox.cpp)
target_link_libraries(SceneSandbox ${libs})

# ImguiSandbox
add_executable(ImguiSandbox ImguiSandbox.cpp)
target_link_libraries(ImguiSandbox ${libs})

# DeliberationSandbox
add_executable(DeliberationSandbox DeliberationSandbox.cpp)
target_link_libraries(DeliberationSandbox ${libs})

