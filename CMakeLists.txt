cmake_minimum_required(VERSION 3.26)
project(ProyectoFinal_CGeIHC)
set(CMAKE_CXX_STANDARD 17)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    if (DEBUG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    elseif (RELEASE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif ()
endif ()

# Requires GLEW, OpenGL and GLFW Packages on Linux
# On Arch Linux: pacman -S glew glfw

# Or Windows require install packages with vcpkg with the following command:
# ./vcpkg install glfw3:x64-windows glfw3:x86-windows glew:x64-windows glew:x86-windows glm:x64-windows glm:x86-windows

# FIND PACKAGES FOR OpenGL, GLFW and GLEW
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    message(STATUS "OpenGL Found")
endif ()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message(STATUS "GLEW Found")
endif ()

find_package(glfw3 REQUIRED)
if (glfw3_FOUND)
    message(STATUS "GLFW Found")
endif ()

find_package(assimp CONFIG REQUIRED)
if (assimp_FOUND)
    message(STATUS "Assimp Found")
endif ()

add_executable(ProyectoFinal_CGeIHC
        src/main.cpp
        src/Window.cpp
        src/Shader.cpp
        src/Shader.cpp
        src/BasicPrimitives.cpp
        src/BasicMesh.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/Camera.cpp
        src/input/KeyboardInput.cpp
        src/input/KeyboardInput.h
)

message(STATUS "Copiando archivos de shader")
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
#message(STATUS "Copiando archivos de modelos")
#file(COPY Models DESTINATION ${CMAKE_BINARY_DIR})
#message(STATUS "Copiando archivos de texturas")
#file(COPY Textures DESTINATION ${CMAKE_BINARY_DIR})

if (UNIX)
    target_link_libraries(ProyectoFinal_CGeIHC glfw OpenGL::GL GLEW assimp::assimp)
else ()
    target_link_libraries(ProyectoFinal_CGeIHC PRIVATE GLEW::GLEW)
    target_link_libraries(ProyectoFinal_CGeIHC PRIVATE glfw)
    target_link_libraries(ProyectoFinal_CGeIHC PRIVATE assimp::assimp)
endif ()