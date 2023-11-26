cmake_minimum_required(VERSION 3.26)
project(ProyectoFinal_CGeIHC)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    message(STATUS "Usando MSVC en Visual Studio")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Usando Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(WARNING "Compilando la versión debug.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -DDEBUG")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DRELEASE")
    endif ()
endif ()

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

find_package(glm CONFIG REQUIRED)
if (glm_FOUND)
    message(STATUS "glm Found")
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
    message(STATUS "STB Found")
endif ()

find_package(Boost REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost found")
endif ()

find_package(nlohmann_json CONFIG REQUIRED)
if (nlohmann_json_FOUND)
    message(STATUS "nlohmann_json Found")
endif ()

include_directories(${Stb_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Animation)
include_directories(${CMAKE_SOURCE_DIR}/Audio)
include_directories(${CMAKE_SOURCE_DIR}/camera)
include_directories(${CMAKE_SOURCE_DIR}/Exceptions)
include_directories(${CMAKE_SOURCE_DIR}/input)
include_directories(${CMAKE_SOURCE_DIR}/Lights)
include_directories(${CMAKE_SOURCE_DIR}/model)
include_directories(${CMAKE_SOURCE_DIR}/Utils)

add_executable(ProyectoFinal_CGeIHC
        src/main.cpp
        src/Window.cpp
        src/Shader.cpp
        src/Shader.cpp
        src/model/Mesh.cpp
        src/model/Model.cpp
        src/camera/Camera.cpp
        src/input/KeyboardInput.cpp
        src/input/KeyboardInput.h
        src/input/MouseInput.cpp
        src/input/MouseInput.h
        src/GlobalConstants.h
        src/camera/OldCameraCollection.cpp
        src/camera/OldCameraCollection.h
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
        src/Lights/Light.cpp
        src/Lights/Light.h
        src/Lights/LightCollection.cpp
        src/Lights/LightCollection.h
        src/Lights/DirectionalLight.cpp
        src/Lights/DirectionalLight.h
        src/Lights/PointLight.cpp
        src/Lights/PointLight.h
        src/Lights/SpotLight.cpp
        src/Lights/SpotLight.h
        src/Lights/PointLight.cpp
        src/Lights/PointLight.h
        src/model/Material.cpp
        src/model/Material.h
        src/Skybox.h
        src/Skybox.cpp
        src/Utils/ImageUtils.cpp
        src/Utils/ImageUtils.h
        src/Utils/Logger.cpp
        src/Utils/Logger.h
        src/model/BoneMesh.cpp
        src/model/BoneMesh.h
        src/model/BoneModel.cpp
        src/model/BoneModel.h
        src/Entity/SimpleEntity.cpp
        src/Entity/SimpleEntity.h
        src/Animation/KeyFrameAnimation.cpp
        src/Animation/Animation.cpp
        src/model/Bone.cpp
        src/model/Bone.h
        src/Utils/Assimp2Glm.cpp
        src/Utils/Assimp2Glm.h
        src/Animation/BoneAnimation.cpp
        src/Animation/BoneAnimation.h
        src/Animation/BoneAnimator.cpp
        src/Animation/BoneAnimator.h
        src/Entity/Entity.cpp
        src/Entity/Entity.h
        src/Entity/Player.cpp
        src/Entity/Player.h
        src/camera/ICamera.h
        src/camera/FreeCamera.cpp
        src/camera/FreeCamera.h
        src/camera/StaticCamera.cpp
        src/camera/StaticCamera.h
        src/camera/PlayerCamera.cpp
        src/camera/PlayerCamera.h
        src/camera/CameraCollection.h
)

message(STATUS "Copiando archivos de shader")
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
message(STATUS "Copiando archivos de modelos")
file(COPY assets/Models DESTINATION ${CMAKE_BINARY_DIR}/assets)
message(STATUS "Copiando archivos de texturas")
file(COPY assets/Textures DESTINATION ${CMAKE_BINARY_DIR}/assets)
message(STATUS "Copiando las animaciones")
file(COPY Animations DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(ProyectoFinal_CGeIHC PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE GLEW::GLEW)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE glfw)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE assimp::assimp)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE OpenAL::OpenAL)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE glm::glm)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(ProyectoFinal_CGeIHC PRIVATE Boost::boost)