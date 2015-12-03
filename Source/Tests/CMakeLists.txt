message(STATUS "Tests")

# target
set(target BanditTest)

# packages
find_package(BANDIT     REQUIRED)
find_package(GLFW       REQUIRED)
find_package(glbinding  REQUIRED)
find_package(globjects  REQUIRED)
find_package(GLM        REQUIRED)

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
    ${GLBINDING_LIBRARY}
)

# executable
add_executable(${target} ${sources})
target_link_libraries(${target} ${libs})

# BasicTriangleTest
add_executable(BasicTriangleTest BasicTriangleTest.cpp)
target_link_libraries(BasicTriangleTest ${libs})

# BasicTextureTest
add_executable(BasicTextureTest BasicTextureTest.cpp)
target_link_libraries(BasicTextureTest ${libs})

