message(STATUS "Sandboxes")

# includes
include_directories(
    BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# DrawSandbox
add_executable(DrawSandbox DrawSandbox.cpp)
target_link_libraries(DrawSandbox Deliberation)

# ECSSandbox
add_executable(ECSSandbox ECSSandbox.cpp)
target_link_libraries(ECSSandbox Deliberation)

# CoreSandbox
add_executable(CoreSandbox CoreSandbox.cpp)
target_link_libraries(CoreSandbox Deliberation)

# SceneSandbox
add_executable(PrototypeSandbox PrototypeSandbox.cpp)
target_link_libraries(PrototypeSandbox Deliberation)

# SceneSandbox
add_executable(SceneSandbox SceneSandbox.cpp)
target_link_libraries(SceneSandbox Deliberation)

# ImguiSandbox
add_executable(ImguiSandbox ImguiSandbox.cpp)
target_link_libraries(ImguiSandbox Deliberation)

# DeliberationSandbox
add_executable(DeliberationSandbox DeliberationSandbox.cpp)
target_link_libraries(DeliberationSandbox Deliberation)

