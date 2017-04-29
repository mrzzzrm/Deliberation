message(STATUS "Deliberation")

# defines
set(include_path 	"${CMAKE_CURRENT_SOURCE_DIR}/include/Deliberation")
set(source_path 	"${CMAKE_CURRENT_SOURCE_DIR}/source")

set(DELIBERATION_INCLUDE_DIRS

    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source

    ${CXXOPTS_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${GLBINDING_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${JSON_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIRS}
    ${BOOST_INCLUDE_DIRS}
    ${TINYOBJLOADER_INCLUDE_DIRS}

    CACHE INTERAL "All Deliberation include dirs" 
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
    ${source_path}/Core/BlobValueAccessor.cpp
    ${source_path}/Core/Color.cpp
    ${source_path}/Core/DataLayout.cpp
    ${source_path}/Core/DataLayoutField.cpp
    ${source_path}/Core/ElementalTypes.cpp
    ${source_path}/Core/FpsCounter.cpp
    ${source_path}/Core/LayoutedBlob.cpp
    ${source_path}/Core/LayoutedBlobElement.cpp
    ${source_path}/Core/FileUtils.cpp
    ${source_path}/Core/InputBase.cpp
    ${source_path}/Core/LapTimer.cpp
    ${source_path}/Core/MainLoop.cpp
    ${source_path}/Core/RandomColorGenerator.cpp
    ${source_path}/Core/ScopeProfiler.cpp
    ${source_path}/Core/StringUtils.cpp
    ${source_path}/Core/Types.cpp
    ${source_path}/Core/Viewport.cpp

    ${source_path}/Core/Math/AABB.cpp
    ${source_path}/Core/Math/Box.cpp
    ${source_path}/Core/Math/GLMUtils.cpp
    ${source_path}/Core/Math/MathUtils.cpp
    ${source_path}/Core/Math/Morton.cpp
    ${source_path}/Core/Math/Ray2D.cpp
    ${source_path}/Core/Math/Ray3D.cpp
    ${source_path}/Core/Math/Rect3D.cpp
    ${source_path}/Core/Math/Plane.cpp
    ${source_path}/Core/Math/PolygonClipping.cpp
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
    ${source_path}/Draw/Detail/QueryImpl.cpp
    ${source_path}/Draw/Detail/QueryImpl.h
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
    ${source_path}/Draw/GL/GLSamplerState.cpp
    ${source_path}/Draw/GL/GLSamplerState.h
    ${source_path}/Draw/GL/GLSLSizeOf.cpp
    ${source_path}/Draw/GL/GLStateManager.cpp
    ${source_path}/Draw/GL/GLType.cpp
    ${source_path}/Draw/GL/GLUtils.cpp
    ${source_path}/Draw/GL/GLUtils.h
    ${source_path}/Draw/GL/GLBindVertexAttribute.cpp
    ${source_path}/Draw/GL/GLBindVertexAttribute.h

    ${source_path}/Draw/BlendState.cpp
    ${source_path}/Draw/Buffer.cpp
    ${source_path}/Draw/BufferUpload.cpp
    ${source_path}/Draw/BufferUploadExecution.cpp
    ${source_path}/Draw/BufferUploadExecution.h
    ${source_path}/Draw/Clear.cpp
    ${source_path}/Draw/ClearExecution.cpp
    ${source_path}/Draw/ClearExecution.h
    ${source_path}/Draw/DrawContext.cpp
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
    ${source_path}/Draw/ProgramInterfaceUniformBlock.cpp
    ${source_path}/Draw/ProgramInterfaceVertexAttribute.cpp
    ${source_path}/Draw/Query.cpp
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
    ${source_path}/Draw/TextureLoaderPixelDataImpl.cpp
    ${source_path}/Draw/TextureLoaderPixelDataImpl.h
    ${source_path}/Draw/TextureLoaderSDLSurfaceImpl.cpp
    ${source_path}/Draw/TextureLoaderSDLSurfaceImpl.h
    ${source_path}/Draw/TextureUpload.cpp
    ${source_path}/Draw/TextureUploadExecution.cpp
    ${source_path}/Draw/TextureUploadExecution.h
    ${source_path}/Draw/Uniform.cpp

    ${source_path}/ECS/ComponentBase.cpp
    ${source_path}/ECS/ComponentFilter.cpp
    ${source_path}/ECS/ComponentSubscriptions.cpp
    ${source_path}/ECS/Entity.cpp
    ${source_path}/ECS/EventManager.cpp
    ${source_path}/ECS/SystemBase.cpp
    ${source_path}/ECS/World.cpp
    ${source_path}/ECS/WorldProfiler.cpp

    ${source_path}/Font/Font.cpp
    ${source_path}/Font/Label.cpp
    ${source_path}/Font/LabelRenderer.cpp
    ${source_path}/Font/MultilineLabel.cpp

    ${source_path}/ImGui/ImGuiSystem.cpp

    ${source_path}/Physics/Contacts/BoxContact.cpp
    ${source_path}/Physics/Contacts/Contact.cpp
    ${source_path}/Physics/Contacts/ContactDispatcher.cpp
    ${source_path}/Physics/Contacts/SphereContact.cpp
    ${source_path}/Physics/BoxShape.cpp
    ${source_path}/Physics/Broadphase.cpp
    ${source_path}/Physics/BroadphaseProxy.cpp
    ${source_path}/Physics/CollisionObject.cpp
    ${source_path}/Physics/CollisionShape.cpp
    ${source_path}/Physics/Narrowphase.cpp
    ${source_path}/Physics/NarrowphasePrimitiveTest.cpp
    ${source_path}/Physics/PhysicsWorld.cpp
    ${source_path}/Physics/ReferenceBroadphase.cpp
    ${source_path}/Physics/RigidBody.cpp
    ${source_path}/Physics/SphereShape.cpp

    ${source_path}/Platform/Application.cpp
    ${source_path}/Platform/Input.cpp
    ${source_path}/Platform/InputLayer.cpp

    ${source_path}/Scene/AmbientOcclusion/AmbientOcclusion.cpp
    ${source_path}/Scene/AmbientOcclusion/AmbientOcclusionBlur.cpp
    ${source_path}/Scene/AmbientOcclusion/AmbientOcclusionMix.cpp
    ${source_path}/Scene/Debug/DebugGeometryManager.cpp
    ${source_path}/Scene/Debug/DebugGeometryRenderer.cpp
    ${source_path}/Scene/Debug/DebugCameraNavigator3D.cpp
    ${source_path}/Scene/Debug/DebugCubemapRenderer.cpp
    ${source_path}/Scene/Debug/DebugGrid3DRenderer.cpp
    ${source_path}/Scene/Debug/DebugGroundPlaneRenderer.cpp
    ${source_path}/Scene/Debug/DebugPhysicsWorldRenderer.cpp
    ${source_path}/Scene/Debug/DebugTexture2dRenderer.cpp
	${source_path}/Scene/Lighting/AmbientLightRenderer.cpp
    ${source_path}/Scene/Model/ModelRenderer.cpp
    ${source_path}/Scene/Pipeline/RenderManager.cpp
    ${source_path}/Scene/Blit.cpp
    ${source_path}/Scene/Camera3D.cpp
    ${source_path}/Scene/CameraDolly3D.cpp
    ${source_path}/Scene/ConeMesh.cpp
    ${source_path}/Scene/CuboidMesh.cpp
    ${source_path}/Scene/EdgeToFaceSmoothing.cpp
    ${source_path}/Scene/HdrRenderer.cpp
    ${source_path}/Scene/Mesh.cpp
    ${source_path}/Scene/MeshData.cpp
    ${source_path}/Scene/MeshCompiler.cpp
    ${source_path}/Scene/ScreenSpaceEffect.cpp
    ${source_path}/Scene/SsaoRenderer.cpp
    ${source_path}/Scene/UVSphere.cpp
    ${source_path}/Scene/SkyboxRenderer.cpp)

# api_includes
set(api_includes
    ${include_path}/Deliberation.h

    ${include_path}/Core/Detail/INamedDataMemberOfImpl.h
    ${include_path}/Core/Detail/INamedDataMemberOfImpl.inl
    ${include_path}/Core/Detail/NamedDataMemberOf.h
    ${include_path}/Core/Detail/NamedDataMemberOf.inl
    ${include_path}/Core/Detail/VectorBlobImpl.h
    ${include_path}/Core/Detail/VectorBlobImpl.inl
    ${include_path}/Core/Detail/IBlobImpl.h

    ${include_path}/Core/Assert.h
    ${include_path}/Core/Blob.h
    ${include_path}/Core/Blob.inl
    ${include_path}/Core/BlobValue.h
    ${include_path}/Core/BlobValue.inl
    ${include_path}/Core/BlobValueAccessor.h
    ${include_path}/Core/BlobValueAccessor.inl
    ${include_path}/Core/BitUtils.h
    ${include_path}/Core/BitUtils.inl
    ${include_path}/Core/Chrono.h
    ${include_path}/Core/Chrono.inl
    ${include_path}/Core/Color.h
    ${include_path}/Core/DataLayout.h
    ${include_path}/Core/DataLayout.inl
    ${include_path}/Core/DataLayoutField.h
    ${include_path}/Core/Dispatch.h
    ${include_path}/Core/Dispatch.inl
    ${include_path}/Core/ElementalTypes.h
    ${include_path}/Core/FileUtils.h
    ${include_path}/Core/FpsCounter.h
    ${include_path}/Core/HashUtils.h
    ${include_path}/Core/HashUtils.inl
    ${include_path}/Core/IntTypes.h
    ${include_path}/Core/InputBase.h
    ${include_path}/Core/LapTimer.h
    ${include_path}/Core/LayoutedBlob.h
    ${include_path}/Core/LayoutedBlob.inl
    ${include_path}/Core/LayoutedBlobElement.h
    ${include_path}/Core/LayoutedBlobElement.inl
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
    ${include_path}/Core/RandomColorGenerator.h
    ${include_path}/Core/RingBuffer.h
    ${include_path}/Core/RingBuffer.inl
    ${include_path}/Core/ScopeProfiler.h
    ${include_path}/Core/StringUtils.h
    ${include_path}/Core/StreamUtils.h
    ${include_path}/Core/StreamUtils.inl
    ${include_path}/Core/TypedBlobValueAccessor.h
    ${include_path}/Core/TypedBlobValueAccessor.inl
    ${include_path}/Core/Span.h
    ${include_path}/Core/Span.inl
    ${include_path}/Core/SparseVector.h
    ${include_path}/Core/SparseVector.inl
    ${include_path}/Core/TypeID.h
    ${include_path}/Core/TypeID.inl
    ${include_path}/Core/Types.h
    ${include_path}/Core/Types.inl
    ${include_path}/Core/Viewport.h

    ${include_path}/Core/Math/AABB.h
    ${include_path}/Core/Math/Box.h
    ${include_path}/Core/Math/GLMUtils.h
    ${include_path}/Core/Math/MathUtils.h
    ${include_path}/Core/Math/Morton.h
    ${include_path}/Core/Math/Morton.inl
    ${include_path}/Core/Math/Plane.h
    ${include_path}/Core/Math/PolygonClipping.h
    ${include_path}/Core/Math/Pose3D.h
    ${include_path}/Core/Math/Pose3D.inl
    ${include_path}/Core/Math/PrimitiveIntersection.h
    ${include_path}/Core/Math/Random.h
    ${include_path}/Core/Math/Random.inl
    ${include_path}/Core/Math/Ray2D.h
    ${include_path}/Core/Math/Ray3D.h
    ${include_path}/Core/Math/Rect3D.h
    ${include_path}/Core/Math/Sphere.h
    ${include_path}/Core/Math/Transform3D.h
    ${include_path}/Core/Math/Vec.h

    ${include_path}/Draw/Detail/UniformImpl.h

    ${include_path}/Draw/GL/GLFramebufferDesc.h
    ${include_path}/Draw/GL/GLSLSizeOf.h
    ${include_path}/Draw/GL/GLStateManager.h
    ${include_path}/Draw/GL/GLSLTypeOf.h
    ${include_path}/Draw/GL/GLSLTypeOf.inl

    ${include_path}/Draw/BlendState.h
    ${include_path}/Draw/Buffer.h
    ${include_path}/Draw/BufferUpload.h
    ${include_path}/Draw/Clear.h
    ${include_path}/Draw/DrawContext.h
    ${include_path}/Draw/CullState.h
    ${include_path}/Draw/DepthState.h
    ${include_path}/Draw/Draw.h
    ${include_path}/Draw/Draw.inl
    ${include_path}/Draw/DrawState.h
    ${include_path}/Draw/Enum.h
    ${include_path}/Draw/Framebuffer.h
    ${include_path}/Draw/PixelFormat.h
    ${include_path}/Draw/Program.h
    ${include_path}/Draw/ProgramInterface.h
    ${include_path}/Draw/ProgramInterfaceFragmentOutput.h
    ${include_path}/Draw/ProgramInterfaceUniform.h
    ${include_path}/Draw/ProgramInterfaceUniformBlock.h
    ${include_path}/Draw/ProgramInterfaceSampler.h
    ${include_path}/Draw/ProgramInterfaceVertexAttribute.h
    ${include_path}/Draw/Query.h
    ${include_path}/Draw/RasterizerState.h
    ${include_path}/Draw/Sampler.h
    ${include_path}/Draw/StencilState.h
    ${include_path}/Draw/Surface.h
    ${include_path}/Draw/SurfaceBinary.h
    ${include_path}/Draw/SurfaceBinary.inl
    ${include_path}/Draw/SurfaceDownload.h
    ${include_path}/Draw/Texture.h
    ${include_path}/Draw/TextureBinary.h
    ${include_path}/Draw/TextureLoader.h
    ${include_path}/Draw/TextureUpload.h
    ${include_path}/Draw/Uniform.h
    ${include_path}/Draw/Uniform.inl

    ${include_path}/ECS/AbstractWorld.h
    ${include_path}/ECS/ComponentFilter.h
    ${include_path}/ECS/Component.h
    ${include_path}/ECS/Component.inl
    ${include_path}/ECS/ComponentBase.h
    ${include_path}/ECS/Defines.h
    ${include_path}/ECS/Entity.h
    ${include_path}/ECS/Entity.inl
    ${include_path}/ECS/EntityComponentSetup.h
    ${include_path}/ECS/EntityData.h
    ${include_path}/ECS/EntityData.inl
    ${include_path}/ECS/EventManager.h
    ${include_path}/ECS/EventManager.inl
    ${include_path}/ECS/SystemBase.h
    ${include_path}/ECS/System.h
    ${include_path}/ECS/World.h
    ${include_path}/ECS/World.inl
    ${include_path}/ECS/WorldProfiler.h

    ${include_path}/Font/Font.h
    ${include_path}/Font/Label.h
    ${include_path}/Font/LabelRenderer.h
    ${include_path}/Font/MultilineLabel.h

    ${include_path}/ImGui/ImGuiSystem.h

    ${include_path}/Physics/Contacts/BoxContact.h
    ${include_path}/Physics/Contacts/Contact.h
    ${include_path}/Physics/Contacts/ContactDispatcher.h
    ${include_path}/Physics/Contacts/ContactDispatcher.inl
    ${include_path}/Physics/Contacts/SphereContact.h
    ${include_path}/Physics/BoxShape.h
    ${include_path}/Physics/Broadphase.h
    ${include_path}/Physics/BroadphaseProxy.h
    ${include_path}/Physics/CollisionObject.h
    ${include_path}/Physics/CollisionShape.h
    ${include_path}/Physics/Narrowphase.h
    ${include_path}/Physics/PhysicsWorld.h
    ${include_path}/Physics/ReferenceBroadphase.h
    ${include_path}/Physics/RigidBody.h
    ${include_path}/Physics/SphereShape.h

    ${include_path}/Platform/Application.h
    ${include_path}/Platform/Input.h
    ${include_path}/Platform/InputEvent.h
    ${include_path}/Platform/InputLayer.h
    ${include_path}/Platform/KeyMap.h
    ${include_path}/Platform/MouseButton.h
    ${include_path}/Platform/MouseButtonEvent.h

    ${include_path}/Scene/AmbientOcclusion/AmbientOcclusion.h
    ${include_path}/Scene/AmbientOcclusion/AmbientOcclusionBlur.h
    ${include_path}/Scene/AmbientOcclusion/AmbientOcclusionMix.h
    ${include_path}/Scene/Debug/DebugGeometryManager.h
    ${include_path}/Scene/Debug/DebugGeometryRenderer.h
    ${include_path}/Scene/Debug/DebugCameraNavigator3D.h
    ${include_path}/Scene/Debug/DebugCubemapRenderer.h
    ${include_path}/Scene/Debug/DebugGrid3DRenderer.h
    ${include_path}/Scene/Debug/DebugGroundPlaneRenderer.h
    ${include_path}/Scene/Debug/DebugPhysicsWorldRenderer.h
    ${include_path}/Scene/Debug/DebugTexture2dRenderer.h
    ${include_path}/Scene/Lighting/AmbientLightRenderer.h
    ${include_path}/Scene/Model/Model.h
    ${include_path}/Scene/Model/ModelInstance.h
    ${include_path}/Scene/Model/ModelRenderer.h
    ${include_path}/Scene/Pipeline/Renderer.cpp
    ${include_path}/Scene/Pipeline/Renderer.h
    ${include_path}/Scene/Pipeline/RenderManager.h
    ${include_path}/Scene/Pipeline/RenderNode.h
    ${include_path}/Scene/Pipeline/RenderPhase.h
    ${include_path}/Scene/Blit.h
    ${include_path}/Scene/Camera3D.h
    ${include_path}/Scene/CameraDolly3D.h
    ${include_path}/Scene/ConeMesh.h
    ${include_path}/Scene/CuboidMesh.h
    ${include_path}/Scene/EdgeToFaceSmoothing.h
    ${include_path}/Scene/HdrRenderer.h
    ${include_path}/Scene/Mesh.h
    ${include_path}/Scene/MeshData.h
    ${include_path}/Scene/MeshCompiler.h
    ${include_path}/Scene/ScreenSpaceEffect.h
    ${include_path}/Scene/SsaoRenderer.h
    ${include_path}/Scene/UVSphere.h
    ${include_path}/Scene/SkyboxRenderer.h)

set(libs
    ${GLFW_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_TTF_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${IMGUI_LIBRARIES}
    ${GLBINDING_LIBRARIES}
    ${TINYOBJLOADER_LIBRARIES}
    )

set(DELIBERATION_LIBRARIES
    Deliberation
    ${libs}
    CACHE INTERNAL ""
)

# library
add_library(Deliberation 
	STATIC
	${api_includes} 
	${sources})
	
target_link_libraries(Deliberation
    ${libs})
	
# include
message("Deliberation include dirs: ${DELIBERATION_INCLUDE_DIRS}")

target_include_directories(Deliberation
    PUBLIC
    ${DELIBERATION_INCLUDE_DIRS}
)
    
target_compile_definitions(Deliberation
    PUBLIC
    DELIBERATION_BUILD_TYPE_DEBUG=${DELIBERATION_BUILD_TYPE_DEBUG}
    DELIBERATION_BUILD_TYPE_RELEASE=${DELIBERATION_BUILD_TYPE_RELEASE}
    )
