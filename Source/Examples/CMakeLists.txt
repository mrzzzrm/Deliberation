message(STATUS "Examples")

# includes
include_directories(
    BEFORE

    ${PROJECT_SOURCE_DIR}/Source/Deliberation/include
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${GLBINDING_INCLUDE_DIR}
    ${BANDIT_INCLUDE_DIR}
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
    ${SDL2_LIBRARY}
    ${SDL2TTF_LIBRARY}
    ${OPENGL_gl_LIBRARY}
)

# AmbientOcclusionExample
add_executable(AmbientOcclusionExample AmbientOcclusionExample.cpp)
target_link_libraries(AmbientOcclusionExample ${libs})

# ECSExample
add_executable(ECSExample ECSExample.cpp)
target_link_libraries(ECSExample ${libs})

# PixelFormatExample
add_executable(PixelFormatExample PixelFormatExample.cpp)
target_link_libraries(PixelFormatExample ${libs})

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

# PhysicsExample
add_executable(PhysicsExample PhysicsExample.cpp)
target_link_libraries(PhysicsExample ${libs})

