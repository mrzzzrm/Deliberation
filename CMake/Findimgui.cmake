set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/Extern/imgui-1.49)

add_library(imgui
    STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/stb_rect_pack.h
    ${IMGUI_DIR}/stb_textedit.h
    ${IMGUI_DIR}/stb_truetype.h
    )

set(IMGUI_LIBRARIES imgui)
set(IMGUI_INCLUDE_DIRS ${IMGUI_DIR})
