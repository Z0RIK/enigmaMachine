# Image loader library
add_library(stb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stb/stb_image.h)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glfw)

# OpenGL loader - GLAD
add_library(glad 
    ${CMAKE_CURRENT_LIST_DIR}/gl/src/gl.c)
target_include_directories(glad PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/gl/include)

# ImGui
add_library(imgui_glfw STATIC
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_tables.cpp

    ${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui_glfw glfw)

target_include_directories(imgui_glfw PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/imgui
    ${CMAKE_CURRENT_LIST_DIR}/glfw/include
)

# Combining all deps in one library
add_library(app_external_deps INTERFACE)

target_link_libraries(app_external_deps INTERFACE
    #legacy_stdio_definitions.lib
    stb
    glad
    imgui_glfw)
