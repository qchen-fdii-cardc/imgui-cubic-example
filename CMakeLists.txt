cmake_minimum_required(VERSION 3.10)
project(opengl_skeleton)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenGL policy and preference
cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Find imgui using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(IMGUI REQUIRED imgui)
pkg_check_modules(SIMPLEINI REQUIRED simpleini)


# Add project source files
set(PROJECT_SOURCES
    main.cpp
    shader.cpp
    camera.cpp
    cube.cpp
    ui.cpp
)

# Add project header files
set(PROJECT_HEADERS
    shader.h
    camera.h
    cube.h
    ui.h
)

# Create executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# https://bugs.launchpad.net/ubuntu/+source/libsimpleini/+bug/1984015
# refuse to Non-free ConvertUTF.h
add_definitions(-DSI_CONVERT_ICU)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IMGUI_INCLUDE_DIRS}  # 添加IMGUI头文件目录
    ${SIMPLEINI_INCLUDE_DIRS}  # 添加SimpleIni头文件目录
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
    ${IMGUI_LIBRARIES}  # 添加IMGUI库
    ${SIMPLEINI_LIBRARIES}  # 添加SimpleIni库
)

# Copy shader files and INI file to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    ${CMAKE_BINARY_DIR}/shaders/vertex
    ${CMAKE_BINARY_DIR}/shaders/fragment
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex
    ${CMAKE_BINARY_DIR}/shaders/vertex
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment
    ${CMAKE_BINARY_DIR}/shaders/fragment
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_config.ini
    ${CMAKE_BINARY_DIR}/
)