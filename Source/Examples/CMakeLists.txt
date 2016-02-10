message(STATUS "Examples")

# packages
find_package(GLFW       REQUIRED)
find_package(glbinding  REQUIRED)
find_package(GLM        REQUIRED)
FIND_PACKAGE(IL		    REQUIRED)
FIND_PACKAGE(ILU	    REQUIRED)
FIND_PACKAGE(SDL2	    REQUIRED)
FIND_PACKAGE(SDL2_ttf   REQUIRED)

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

# BasicCoreExample
add_executable(BasicCoreExample BasicCoreExample.cpp)
target_link_libraries(BasicCoreExample ${libs})

# BasicMathExample
add_executable(BasicMathExample BasicMathExample.cpp)
target_link_libraries(BasicMathExample ${libs})

# BasicTriangleExample
add_executable(BasicTriangleExample BasicTriangleExample.cpp)
target_link_libraries(BasicTriangleExample ${libs})

# BasicTextureExample
add_executable(BasicTextureExample BasicTextureExample.cpp)
target_link_libraries(BasicTextureExample ${libs})

# BasicFontExample
add_executable(BasicFontExample BasicFontExample.cpp)
target_link_libraries(BasicFontExample ${libs})

# BasicRenderTargetExample
add_executable(BasicRenderTargetExample BasicRenderTargetExample.cpp)
target_link_libraries(BasicRenderTargetExample ${libs})

# BasicSceneExample
add_executable(BasicSceneExample BasicSceneExample.cpp)
target_link_libraries(BasicSceneExample ${libs})

