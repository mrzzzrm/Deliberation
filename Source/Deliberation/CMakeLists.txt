message(STATUS "Deliberation")

# defines
set(include_path 	"${CMAKE_CURRENT_SOURCE_DIR}/include/Deliberation")
set(source_path 	"${CMAKE_CURRENT_SOURCE_DIR}/source")

# packages
find_package(glbinding  REQUIRED)
find_package(GLM        REQUIRED)
FIND_PACKAGE(IL		    REQUIRED)
FIND_PACKAGE(ILU	    REQUIRED)
FIND_PACKAGE(SDL2	    REQUIRED)
FIND_PACKAGE(SDL2_ttf   REQUIRED)
find_package(GLFW       REQUIRED)

# include
include_directories(
    BEFORE

    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source

    ${SDL2TTF_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${IL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
)

# library type
if (OPTION_BUILD_STATIC)
    add_definitions("-DDELIBERATION_STATIC")
else()
    add_definitions("-DDELIBERATION_EXPORTS")
endif()

# sources
set(sources
    ${source_path}/Deliberation.cpp

    ${source_path}/Core/Assert.cpp
    ${source_path}/Core/BitUtils.cpp
    ${source_path}/Core/Blob.cpp
    ${source_path}/Core/Color.cpp
    ${source_path}/Core/FileUtils.cpp
    ${source_path}/Core/InputAdapterBase.cpp
    ${source_path}/Core/LapTimer.cpp
    ${source_path}/Core/MainLoop.cpp
    ${source_path}/Core/StringUtils.cpp
    ${source_path}/Core/Viewport.cpp

    ${source_path}/Core/Math/AABB.cpp
    ${source_path}/Core/Math/GLMUtils.cpp
    ${source_path}/Core/Math/Morton.cpp
    ${source_path}/Core/Math/Ray3D.cpp
    ${source_path}/Core/Math/Rect3D.cpp
    ${source_path}/Core/Math/Plane.cpp
    ${source_path}/Core/Math/PrimitiveIntersection.cpp
    ${source_path}/Core/Math/Sphere.cpp
    ${source_path}/Core/Math/Transform3D.cpp

    ${source_path}/Draw/Detail/ClearImpl.cpp
    ${source_path}/Draw/Detail/ClearImpl.h
    ${source_path}/Draw/Detail/BufferBinding.h
    ${source_path}/Draw/Detail/BufferImpl.cpp
    ${source_path}/Draw/Detail/BufferImpl.h
    ${source_path}/Draw/Detail/DrawExecution.cpp
    ${source_path}/Draw/Detail/DrawExecution.h
    ${source_path}/Draw/Detail/DrawImpl.cpp
    ${source_path}/Draw/Detail/DrawImpl.h
    ${source_path}/Draw/Detail/FramebufferImpl.cpp
    ${source_path}/Draw/Detail/FramebufferImpl.h
    ${source_path}/Draw/Detail/ProgramImpl.cpp
    ${source_path}/Draw/Detail/ProgramImpl.h
    ${source_path}/Draw/Detail/SamplerImpl.cpp
    ${source_path}/Draw/Detail/SamplerImpl.h
    ${source_path}/Draw/Detail/SurfaceDownloadImpl.cpp
    ${source_path}/Draw/Detail/SurfaceDownloadImpl.h
    ${source_path}/Draw/Detail/TextureImpl.cpp
    ${source_path}/Draw/Detail/TextureImpl.h
    ${source_path}/Draw/Detail/UniformImpl.cpp

    ${source_path}/Draw/GL/GLFramebufferDesc.cpp
    ${source_path}/Draw/GL/GLFramebuffer.cpp
    ${source_path}/Draw/GL/GLFramebuffer.h
    ${source_path}/Draw/GL/GLSLSizeOf.cpp
    ${source_path}/Draw/GL/GLStateManager.cpp
    ${source_path}/Draw/GL/GLUtils.cpp
    ${source_path}/Draw/GL/GLUtils.h
    ${source_path}/Draw/GL/GLVertexAttributeBinder.cpp
    ${source_path}/Draw/GL/GLVertexAttributeBinder.h
    ${source_path}/Draw/GL/GLVertexAttributeFormat.cpp
    ${source_path}/Draw/GL/GLVertexAttributeFormat.h

    ${source_path}/Draw/BlendState.cpp
    ${source_path}/Draw/Buffer.cpp
    ${source_path}/Draw/BufferLayout.cpp
    ${source_path}/Draw/BufferLayoutField.cpp
    ${source_path}/Draw/BufferUpload.cpp
    ${source_path}/Draw/BufferUploadExecution.cpp
    ${source_path}/Draw/BufferUploadExecution.h
    ${source_path}/Draw/Clear.cpp
    ${source_path}/Draw/ClearExecution.cpp
    ${source_path}/Draw/ClearExecution.h
    ${source_path}/Draw/Context.cpp
    ${source_path}/Draw/CullState.cpp
    ${source_path}/Draw/DepthState.cpp
    ${source_path}/Draw/Draw.cpp
    ${source_path}/Draw/DrawVerification.cpp
    ${source_path}/Draw/DrawVerification.h
    ${source_path}/Draw/DrawState.cpp
    ${source_path}/Draw/Framebuffer.cpp
    ${source_path}/Draw/ILUtils.cpp
    ${source_path}/Draw/ILUtils.h
    ${source_path}/Draw/ITextureLoaderImpl.cpp
    ${source_path}/Draw/ITextureLoaderImpl.h
    ${source_path}/Draw/PixelFormat.cpp
    ${source_path}/Draw/Program.cpp
    ${source_path}/Draw/ProgramInterface.cpp
    ${source_path}/Draw/ProgramInterfaceFragmentOutput.cpp
    ${source_path}/Draw/ProgramInterfaceSampler.cpp
    ${source_path}/Draw/ProgramInterfaceUniform.cpp
    ${source_path}/Draw/ProgramInterfaceVertexAttribute.cpp
    ${source_path}/Draw/RasterizerState.cpp
    ${source_path}/Draw/Sampler.cpp
    ${source_path}/Draw/StencilState.cpp
    ${source_path}/Draw/StencilState.cpp
    ${source_path}/Draw/Surface.cpp
    ${source_path}/Draw/SurfaceBinary.cpp
    ${source_path}/Draw/SurfaceDownload.cpp
    ${source_path}/Draw/Texture.cpp
    ${source_path}/Draw/TextureBinary.cpp
    ${source_path}/Draw/TextureLoader.cpp
    ${source_path}/Draw/TextureLoaderCubeFilesImpl.cpp
    ${source_path}/Draw/TextureLoaderCubeFilesImpl.h
    ${source_path}/Draw/TextureLoaderDataImpl.cpp
    ${source_path}/Draw/TextureLoaderDataImpl.h
    ${source_path}/Draw/TextureLoaderFileImpl.cpp
    ${source_path}/Draw/TextureLoaderFileImpl.h
    ${source_path}/Draw/TextureLoaderSDLSurfaceImpl.cpp
    ${source_path}/Draw/TextureLoaderSDLSurfaceImpl.h
    ${source_path}/Draw/TextureUpload.cpp
    ${source_path}/Draw/TextureUploadExecution.cpp
    ${source_path}/Draw/TextureUploadExecution.h
    ${source_path}/Draw/Uniform.cpp

    ${source_path}/Font/Font.cpp
    ${source_path}/Font/Label.cpp
    ${source_path}/Font/LabelRenderer.cpp

    ${source_path}/Scene/Camera3D.cpp
    ${source_path}/Scene/DebugCameraNavigator3D.cpp
    ${source_path}/Scene/DebugGrid3DRenderer.cpp
    ${source_path}/Scene/UVSphere.cpp
)

# api_includes
set(api_includes
    ${include_path}/Deliberation.h
    ${include_path}/Deliberation_API.h

    ${include_path}/Core/Detail/IBlobImpl.h
    ${include_path}/Core/Assert.h
    ${include_path}/Core/Blob.h
    ${include_path}/Core/Blob.inl
    ${include_path}/Core/BitUtils.h
    ${include_path}/Core/BitUtils.inl
    ${include_path}/Core/Color.h
    ${include_path}/Core/FileUtils.h
    ${include_path}/Core/HashUtils.h
    ${include_path}/Core/HashUtils.inl
    ${include_path}/Core/IntTypes.h
    ${include_path}/Core/InputAdapterBase.h
    ${include_path}/Core/LapTimer.h
    ${include_path}/Core/LinearMap.h
    ${include_path}/Core/LinearMap.inl
    ${include_path}/Core/LinearOctree.h
    ${include_path}/Core/LinearOctree.inl
    ${include_path}/Core/LapTimer.h
    ${include_path}/Core/MainLoop.h
    ${include_path}/Core/OffsetOf.h
    ${include_path}/Core/OffsetOf.inl
    ${include_path}/Core/Optional.h
    ${include_path}/Core/Optional.inl
    ${include_path}/Core/RingBuffer.h
    ${include_path}/Core/RingBuffer.inl
    ${include_path}/Core/StringUtils.h
    ${include_path}/Core/StreamUtils.h
    ${include_path}/Core/StreamUtils.inl
    ${include_path}/Core/Viewport.h

    ${include_path}/Core/Math/AABB.h
    ${include_path}/Core/Math/GLMUtils.h
    ${include_path}/Core/Math/Morton.h
    ${include_path}/Core/Math/Morton.inl
    ${include_path}/Core/Math/Plane.h
    ${include_path}/Core/Math/PrimitiveIntersection.h
    ${include_path}/Core/Math/Ray3D.h
    ${include_path}/Core/Math/Rect3D.h
    ${include_path}/Core/Math/Sphere.h
    ${include_path}/Core/Math/Transform3D.h

    ${include_path}/Draw/Detail/INamedDataMemberOfImpl.h
    ${include_path}/Draw/Detail/INamedDataMemberOfImpl.inl
    ${include_path}/Draw/Detail/NamedDataMemberOf.h
    ${include_path}/Draw/Detail/NamedDataMemberOf.inl
    ${include_path}/Draw/Detail/UniformImpl.h

    ${include_path}/Draw/GL/GLFramebufferDesc.h
    ${include_path}/Draw/GL/GLSLSizeOf.h
    ${include_path}/Draw/GL/GLStateManager.h
    ${include_path}/Draw/GL/GLSLTypeOf.h
    ${include_path}/Draw/GL/GLSLTypeOf.inl

    ${include_path}/Draw/BlendState.h
    ${include_path}/Draw/Buffer.h
    ${include_path}/Draw/BufferLayout.h
    ${include_path}/Draw/BufferLayoutField.h
    ${include_path}/Draw/BufferUpload.h
    ${include_path}/Draw/Clear.h
    ${include_path}/Draw/Context.h
    ${include_path}/Draw/Context.inl
    ${include_path}/Draw/CullState.h
    ${include_path}/Draw/DepthState.h
    ${include_path}/Draw/Draw.h
    ${include_path}/Draw/Draw.inl
    ${include_path}/Draw/DrawState.h
    ${include_path}/Draw/Framebuffer.h
    ${include_path}/Draw/PixelFormat.h
    ${include_path}/Draw/Program.h
    ${include_path}/Draw/ProgramInterface.h
    ${include_path}/Draw/ProgramInterfaceFragmentOutput.h
    ${include_path}/Draw/ProgramInterfaceUniform.h
    ${include_path}/Draw/ProgramInterfaceSampler.h
    ${include_path}/Draw/ProgramInterfaceVertexAttribute.h
    ${include_path}/Draw/RasterizerState.h
    ${include_path}/Draw/Sampler.h
    ${include_path}/Draw/StencilState.h
    ${include_path}/Draw/Surface.h
    ${include_path}/Draw/SurfaceBinary.h
    ${include_path}/Draw/SurfaceDownload.h
    ${include_path}/Draw/Texture.h
    ${include_path}/Draw/TextureBinary.h
    ${include_path}/Draw/TextureLoader.h
    ${include_path}/Draw/TextureUpload.h
    ${include_path}/Draw/Uniform.h
    ${include_path}/Draw/Uniform.inl

    ${include_path}/Font/Font.h
    ${include_path}/Font/Label.h
    ${include_path}/Font/LabelRenderer.h

    ${include_path}/Scene/Camera3D.h
    ${include_path}/Scene/DebugCameraNavigator3D.h
    ${include_path}/Scene/DebugGrid3DRenderer.h
    ${include_path}/Scene/Mesh.h
    ${include_path}/Scene/Mesh.inl
    ${include_path}/Scene/MeshCompiler.h
    ${include_path}/Scene/MeshCompiler.inl
    ${include_path}/Scene/UVSphere.h
)

# libs
set(libs
    ${GLFW_LIBRARY}
    ${GLBINDING_LIBRARY}
    ${IL_LIBRARIES}
    ${ILU_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2TTF_LIBRARY}
)

# library
add_library(Deliberation ${api_includes} ${sources})
target_link_libraries(Deliberation ${libs})
