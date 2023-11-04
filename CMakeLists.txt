cmake_minimum_required(VERSION 3.26)
project(ProyectoFinal_CGeIHC)
set(CMAKE_CXX_STANDARD 17)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "Compilando la versión debug.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -DDEBUG")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DRELEASE")
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

find_package(OpenAL CONFIG REQUIRED)
if (OpenAL_FOUND)
    message(STATUS "OpenAL Found")
endif ()

find_package(Stb REQUIRED)
if (Stb_FOUND)
    message(STATUS "STB Found at ${Stb_INCLUDE_DIR}")
endif ()

include_directories(${Stb_INCLUDE_DIR})

add_executable(ProyectoFinal_CGeIHC
        src/main.cpp
        src/Window.cpp
        src/Shader.cpp
        src/Shader.cpp
        src/model/BasicPrimitives.cpp
        src/model/BasicMesh.cpp
        src/model/Mesh.cpp
        src/model/Model.cpp
        src/camera/Camera.cpp
        src/input/KeyboardInput.cpp
        src/input/KeyboardInput.h
        src/input/MouseInput.cpp
        src/input/MouseInput.h
        src/GlobalConstants.h
        src/camera/CameraCollection.cpp
        src/camera/CameraCollection.h
        src/Utils/ModelMatrix.cpp
        src/Utils/ModelMatrix.h
        src/Audio/AudioController.cpp
        src/Audio/AudioController.h
        src/Audio/AudioDevice.cpp
        src/Audio/AudioDevice.h
        src/model/ModelCollection.cpp
        src/model/ModelCollection.h
        src/model/Texture.cpp
        src/model/Texture.h
        src/Utils/PathUtils.cpp
        src/Utils/PathUtils.h
)

message(STATUS "Copiando archivos de shader")
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
message(STATUS "Copiando archivos de modelos")
file(COPY assets/Models DESTINATION ${CMAKE_BINARY_DIR}/assets)
message(STATUS "Copiando archivos de texturas")
file(COPY assets/Textures DESTINATION ${CMAKE_BINARY_DIR}/assets)

target_include_directories(ProyectoFinal_CGeIHC PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE GLEW::GLEW)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE glfw)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE assimp::assimp)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE OpenAL::OpenAL)
#target_link_libraries(ProyectoFinal_CGeIHC PRIVATE OpenMP)
