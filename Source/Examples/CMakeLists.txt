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

add_library(ExampleCommon
    SceneExampleApplication.cpp
    SceneExampleApplication.h
)

target_link_libraries(ExampleCommon
    ${DELIBERATION_LIBRARIES}
)

# libs
set(libs
    ExampleCommon
)

# AmbientOcclusionExample
add_executable(AmbientOcclusionExample AmbientOcclusionExample.cpp)
target_link_libraries(AmbientOcclusionExample ${libs})

# DrawSandbox
add_executable(DrawSandbox DrawSandbox.cpp)
target_link_libraries(DrawSandbox ${libs})

# BoxBoxSandbox
add_executable(BoxBoxSandbox BoxBoxSandbox.cpp)
target_link_libraries(BoxBoxSandbox ${libs})

# ECSExample
add_executable(ECSExample ECSExample.cpp)
target_link_libraries(ECSExample ${libs})

# DebugGeometryRendererExample
add_executable(DebugGeometryRendererExample DebugGeometryRendererExample.cpp)
target_link_libraries(DebugGeometryRendererExample ${libs})

# PixelFormatExample
add_executable(PixelFormatExample PixelFormatExample.cpp)
target_link_libraries(PixelFormatExample ${libs})

# BasicCoreExample
add_executable(BasicCoreExample BasicCoreExample.cpp)
target_link_libraries(BasicCoreExample ${libs})

# BasicMathExample
add_executable(BasicMathExample BasicMathExample.cpp)
target_link_libraries(BasicMathExample ${libs})

# BasicMeshExample
add_executable(BasicMeshExample BasicMeshExample.cpp)
target_link_libraries(BasicMeshExample ${libs})

# BasicTriangleExample
add_executable(BasicTriangleExample BasicTriangleExample.cpp)
target_link_libraries(BasicTriangleExample ${libs})

# BasicTextureExample
add_executable(BasicTextureExample BasicTextureExample.cpp)
target_link_libraries(BasicTextureExample ${libs})

# BasicFontExample
add_executable(BasicFontExample BasicFontExample.cpp)
target_link_libraries(BasicFontExample ${libs})

# BasicPhysicsExample
add_executable(BasicPhysicsExample BasicPhysicsExample.cpp)
target_link_libraries(BasicPhysicsExample ${libs})

# BasicRenderTargetExample
add_executable(BasicRenderTargetExample BasicRenderTargetExample.cpp)
target_link_libraries(BasicRenderTargetExample ${libs})

# BasicSceneExample
add_executable(BasicSceneExample BasicSceneExample.cpp)
target_link_libraries(BasicSceneExample ${libs})

# DrawExample
add_executable(DrawExample DrawExample.cpp)
target_link_libraries(DrawExample ${libs})

# PhysicsExample
add_executable(PhysicsExample PhysicsExample.cpp)
target_link_libraries(PhysicsExample ${libs})

# EdgeToFaceSmoothingExample
add_executable(EdgeToFaceSmoothingExample EdgeToFaceSmoothingExample.cpp)
target_link_libraries(EdgeToFaceSmoothingExample ${libs})

# ImguiSandbox
add_executable(ImguiSandbox ImguiSandbox.cpp)
target_link_libraries(ImguiSandbox ${libs})

