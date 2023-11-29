#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION
#define MARBLE_ANIM_FILE "Animations/marble.json"

#define ToRGB(col) (col / 255.0f)

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Animation/Animation.h"
#include "Animation/BoneAnimation.h"
#include "Animation/BoneAnimator.h"
#include "Animation/KeyFrameAnimation.h"
#include "Audio/AudioDevice.h"
#include "Entity/Player.h"
#include "Entity/SimpleEntity.h"
#include "GlobalConstants.h"
#include "Lights/DirectionalLight.h"
#include "Lights/LightCollection.h"
#include "Skybox.h"
#include "Utils/ModelMatrix.h"
#include "Window.h"
#include "camera/CameraCollection.h"
#include "camera/FreeCamera.h"
#include "camera/ICamera.h"
#include "camera/PlayerCamera.h"
#include "camera/StaticCamera.h"
#include "input/KeyboardInput.h"
#include "model/BoneModel.h"
#include "model/Material.h"
#include "model/ModelCollection.h"
#include "model/Texture.h"
#include <boost/format.hpp>

#include "Audio/AudioController.h"
#include "Audio/AudioSource.h"
#include "model/MeshPrimitive.h"
// #include "Window.h"
// src/glew.h
// src / Mesh.h src / Mesh_tn.cpp
//  region Global Variables
Window mainWindow;
// #warning "Old camera implementation"
// Camera::OldCameraCollection oldCameras;
// Camera::Camera *oldActiveCamera;

Camera::CameraCollection<Camera::ICamera> cameras;
Camera::ICamera *activeCamera;

Model::ModelCollection models;
Lights::LightCollection<Lights::DirectionalLight> directionalLights;
Lights::LightCollection<Lights::PointLight> pointLights;
Lights::LightCollection<Lights::SpotLight> spotLights;
Skybox skyboxDay;
Skybox skyboxNight;
Skybox *skyBoxCurrent;
Entity::SimpleEntity marbleEntity;
Animation::KeyFrameAnimation marbleKfAnim;
Animation::Animation marblePreLaunch;
Entity::Player avatarPlayer(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
glm::vec3 leverPos = {-85.369f, 43.931f, 36.921f};
const glm::vec3 leverEnd = {-90.089f, 42.213f, 36.921f};
const glm::vec3 leverDirection = glm::normalize(leverEnd - leverPos);
Model::BoneModel avatar(Utils::PathUtils::getModelsPath().append("/2b_walk_static.fbx"));
/*
 * Fuente audios:
 * Rolling ball: https://opengameart.org/content/bowling-ball-rolling
 */
Audio::AudioSource mblKeyframesAudio(Utils::PathUtils::getAudioPath().append("/rolling_marble.wav"), true);
Audio::AudioSource mblSaAudio(Utils::PathUtils::getAudioPath().append("/rolling_marble.wav"), true);
Audio::AudioSource walkingAudio(Utils::PathUtils::getAudioPath().append("/HeelSoundEffect.wav"), true);

// Constantes para uniforms
GLuint uProjection, uModel, uView, uEyePosition, uSpecularIntensity, uShininess, uTexOffset, uColor, uBonesMatrices;

Model::Material matMetal;
Model::Material Material_brillante;
Model::Material Material_opaco;
Model::Texture primitiva;

float timer = 0.0f;

std::unordered_map<int, Shader *> shaders;
Animation::Animation CanicaAS;
Animation::Animation CanicaAS2;
Animation::Animation PicoJerarquia1;
Animation::Animation PicoJerarquia2;
Animation::Animation PicoJerarquia3;
// Animation::Animation modeloJerarquico1;
bool mJ1_trigger = false;

// Posicion numeros: 60.3627, 115.599, -34.7832

float deltaTime = 0.0f;
float deltaTimeAnim = 0.0f;
float lastTime = 0.0f;
const float limitFPS = 1.0f / 60.0f;

AMB_LIGHTS ambLight = AMB_LIGHTS::DAY;
// endregion

// region Variables auxiliares que no se encapsularon en otras clases :v
int globalCounter = 0; // Temporal xd
float rightFlipperRotation = 0.0f;
float leftFlipperRotation = 0.0f;
float upFlipperRotation = 0.0f;
bool captureMode = false;
float expandeResorte = 1.0f;
bool CambioAnim = false;
bool activarP1 = false, activarP2 = false, activarP3 = false; // nuevo
float rotOffPico = 3.5;
float escOffPico = .01;
float rotPico1 = 0;
float rotPicoZ1 = 0;
float escPico1 = 0;
float rotPico2 = 0;
float rotPicoZ2 = 0;
float escPico2 = 0;
float rotPico3 = 0;
float rotPicoZ3 = 0;
float escPico3 = 0;

std::vector<MeshPrimitive *> meshListPrimitive;

bool activarCanicaSimple = false;
bool activarCanicaSimple2 = false;
float Cx = -78.0, Cy = 46.3, Cz = 7.5; // orig

// float Cx = 11.1, Cy = 55.4, Cz =4.0; p1
// float Cx = 25.1, Cy = 57.3, Cz = 1.0;
// float Cx = 5.2, Cy = 55.1, Cz = -10.1;

// float Cx = 26.3, Cy = 57.4, Cz = -23.5;// toca pico
// float Cx2 = 21.3, Cy2 = 56.9, Cz2 = -3.5;
// float Cx2 = -40.3, Cy2 = 50.5, Cz2 = 9.9;
// float Cx = -52.0, Cy = 49.25, Cz = -10.3;
// float Cx2 = -32.3, Cy2 = 51.2, Cz2 = -5.5;
// float Cx = -78.0, Cy = 46.3, Cz = -5.5;
// float Cx2 = -78.0,Cy2=46.3,Cz2= 7.5;

float AnteriorX, AnteriorY, AnteriorZ;
float NuevoX, NuevoY, NuevoZ;
float rotOffCanica = 0.6;
float movOffCanica = 0.02;
float rotXCanica = 0.0;
float rotZCanica = 0.0;
float t = 0;
int flagC = 0;
glm::vec3 canicaASPos = {Cx, Cy, Cz};
/* float movXCanica = 0;
float movYCanica = 0;
float movZCanica = 0;
float movXOffCanica = 0.4;
float movYOffCanica = 0.06;
float movZOffCanica = 0.2;
float movYImpulso = 0.0524;
float movXImpulso = 0.5;

float rotOffCanica = 1.5;
int BCan = 0; // bandera canica
float t = 0;
float i = 0;*/

// #define MJ_BASE_ROT(n) glm::vec3 MjBaseRot##n = {0.0f, 0.0f, 0.0f};
//  n = Numero de articulacion
#define MJ_ROT(n) glm::vec3 MjRot_##n = {0.0f, 0.0f, 0.0f};
#define MJ_POS(n) glm::vec3 MjPos_##n = {0.0f, 0.0f, 0.0f};

// Base del modelo
MJ_ROT(0)
MJ_ROT(2)
MJ_ROT(3)
MJ_ROT(4)
MJ_POS(0)
MJ_POS(1)
MJ_POS(2)
MJ_POS(3)

// endregion

void InitKeymaps()
{
	Input::KeyboardInput::GetInstance()
	    .createKeymap(KEYMAPS::FREE_CAMERA)
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_ESCAPE,
	        []() -> void
	        {
		        glfwSetWindowShouldClose(mainWindow.getWindowPointer(), GL_TRUE);
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_TAB,
	        []() -> void
	        {
		        activeCamera = cameras.switchCamera();
		        if (typeid(*activeCamera) == typeid(Camera::PlayerCamera))
			        avatarPlayer.setEnableControls(true);
		        else
			        avatarPlayer.setEnableControls(false);
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_I, ///////////nuevo
	        []() -> void
	        {
		        activarP1 = true;
		        PicoJerarquia1.start();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_U,
	        []() -> void
	        {
		        activarP2 = true;
		        PicoJerarquia2.start();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_O,
	        []() -> void
	        {
		        activarP3 = true;
		        PicoJerarquia3.start();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_P, ///////////////
	        []() -> void
	        {
		        activarCanicaSimple = true;
		        CanicaAS.start();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_T,
	        []() -> void
	        {
		        std::cout << "Mouse disabled!";
		        Input::MouseInput::GetInstance().toggleMouseEnabled();
		        mainWindow.toggleMouse();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_0,
	        []() -> void
	        {
		        captureMode = true;
		        Input::KeyboardInput::GetInstance().setKeymap(KEYMAPS::KEYFRAME_CAPTURE);
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_1,
	        []() -> void
	        {
		        pointLights.toggleLight(3, !pointLights.getLightStatus(3));
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_2,
	        []() -> void
	        {
		        pointLights.toggleLight(4, !pointLights.getLightStatus(4));
	        });

	Input::KeyboardInput::GetInstance()
	    .createKeymap(KEYMAPS::KEYFRAME_CAPTURE)
	    .addCallback(
	        KEYMAPS::KEYFRAME_CAPTURE, GLFW_KEY_T,
	        []() -> void
	        {
		        std::cout << "Mouse disabled!";
		        Input::MouseInput::GetInstance().toggleMouseEnabled();
		        mainWindow.toggleMouse();
	        })
	    .addCallback(KEYMAPS::KEYFRAME_CAPTURE,
	                 GLFW_KEY_G,
	                 []() -> void
	                 {
		                 marbleKfAnim.saveToFile(MARBLE_ANIM_FILE);
	                 })
	    .addCallback(KEYMAPS::KEYFRAME_CAPTURE,
	                 GLFW_KEY_C,
	                 []() -> void
	                 {
		                 marbleKfAnim.addKeyframe(marbleEntity.getPosition(), marbleEntity.getRotation());
	                 })
	    .addCallback(KEYMAPS::KEYFRAME_CAPTURE,
	                 GLFW_KEY_R,
	                 []() -> void
	                 {
		                 marbleKfAnim.removeLastFrame();
	                 })
	    .addCallback(
	        KEYMAPS::KEYFRAME_CAPTURE, GLFW_KEY_0,
	        []() -> void
	        {
		        captureMode = false;
		        Input::KeyboardInput::GetInstance().setKeymap(KEYMAPS::FREE_CAMERA);
	        })
	    .addCallback(
	        KEYMAPS::KEYFRAME_CAPTURE,
	        GLFW_KEY_SPACE,
	        []() -> void
	        {
		        if (!captureMode)
			        marbleKfAnim.play();
	        });

	Input::MouseInput::GetInstance()
	    .createKeymap(KEYMAPS::FREE_CAMERA)
	    .addClickCallback(
	        KEYMAPS::FREE_CAMERA,
	        GLFW_MOUSE_BUTTON_RIGHT, // Expansion del resorte
	        []() -> void
	        {
		        marblePreLaunch.start();
	        },
	        true,
	        []() -> void
	        {
		        activarP1 = false, activarP2 = false, activarP3 = false; // nuevo
		        rotPico1 = 0, rotPicoZ1 = 0, escPico1 = 0;
		        rotPico2 = 0, rotPicoZ2 = 0, escPico2 = 0;
		        rotPico3 = 0, rotPicoZ3 = 0, escPico3 = 0;
		        if (!captureMode && !CambioAnim)
		        {
			        CambioAnim = true;
			        marbleKfAnim.play();
		        }
		        if (flagC == 1 && CambioAnim)
		        {

			        CambioAnim = false;
			        activarCanicaSimple2 = true;
			        CanicaAS2.start();
		        }
	        })
	    .addMoveCallback(
	        KEYMAPS::FREE_CAMERA,
	        [](float) -> void
	        {
		        activeCamera->MouseControl(Input::MouseInput::GetInstance());
	        });
}

void CrearPrimitiva()
{
	unsigned int cubo_indices[] = {
	    0,1,2,
	    3,4,5,
		6,7,8,
		9,10,11,
	    12,13,14,

	    15,16,17,
		18,19,20,
		21,21,23,
		24,25,26,
		27,28,29,
	    
		30,31,32,
		33,34,35,
		36,37,38,
		39,40,41,
		42,43,44,

		45,46,47,
		48,49,50,
		51,52,53,
		54,55,56,
		57,58,59,
		
		60,61,62,
		63,64,65,
		66,67,68,
		69,70,71,
		72,73,74,

		75,76,77,
		78,79,80,
		81,82,83,
		84,85,86,
		87,88,89,

		90,91,92,
		93,94,95,
		96,97,98,
		99,100,101,
		102,103,104,

		105,106,107,
		108,109,110,
		111,112,113,
	};

	GLfloat cubo_vertices[] = {
	    // x	y		z		S		T			NX		NY		NZ
		///lados
		-4.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
	    -2.0f, 1.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,

	    -4.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, 1.0f,		0.1f, 0.0f,		0.0f, 0.0f, -1.0f,
	    -4.0f, 1.0f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		-2.0f, 1.0f, 1.0f,		1.0f, 1.00f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, 3.5f,		0.00f, 0.0f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 1.0f, 3.5f,		0.00f, 1.0f, 0.0f, 0.0f, -1.0f,

	    -2.0f, 1.0f, 1.0f,		0.0f, 1.00f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, 1.0f,		1.0f, 0.00f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, 3.5f,		1.00f, 0.0f, 0.0f, 0.0f, -1.0f,
		//////
		-2.0f, 1.0f, 3.5f,		0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     0.0f, 0.0f, 2.0f,		0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 0.0f, 0.0f, 2.0f,		0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     0.0f, 1.0f, 2.0f,		0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 1.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		-2.0f, 1.0f, 3.5f,		0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     0.0f, 0.0f, 2.0f,		0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 0.0f, 0.0f, 2.0f,		0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     0.0f, 1.0f, 2.0f,		0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 1.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,


		 0.0f, 1.0f, 2.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 0.0f, 0.0f, 2.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 2.0f, 0.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 2.0f, 0.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 2.0f, 1.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 0.0f, 1.0f, 2.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 2.0f, 0.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 2.0f, 1.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 2.0f, 1.0f, 1.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 2.0f, 1.0f, 1.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 2.0f, 0.0f, 1.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 2.0f, 0.0f, 3.5f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 //
		 2.0f, 1.0f, 1.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 2.0f, 0.0f, 1.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 4.0f, 1.0f, 0.0f,		0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 4.0f, 0.0f, 0.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 4.0f, 1.0f, 0.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 2.0f, 0.0f, 1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
	    //
		 4.0f, 1.0f, 0.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     2.0f, 0.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
	     2.0f, 1.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

	     4.0f, 0.0f, 0.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     2.0f, 0.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
	     4.0f, 1.0f, 0.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,

		2.0f, 1.0f, -1.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    2.0f, 0.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
	    2.0f, 1.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

	    2.0f, 1.0f, -1.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    2.0f, 0.0f, -1.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    2.0f, 0.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		//
		2.0f, 1.0f, -3.5f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    0.0f, 0.0f, -2.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	    2.0f, 0.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 0.0f, 0.0f, -2.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     0.0f, 1.0f, -2.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     2.0f, 1.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 -2.0f, 1.0f, -3.5f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     0.0f, 0.0f, -2.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     -2.0f, 0.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 0.0f, 0.0f, -2.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     0.0f, 1.0f, -2.0f, 0.5f, 0.75f, 0.0f, 0.0f, -1.0f,
	     -2.0f, 1.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 //
		 -2.0f, 0.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 -2.0f, 1.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 -2.0f, 1.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 -2.0f, 1.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 -2.0f, 0.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 -2.0f, 0.0f, -3.5f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 
		 -2.0f, 1.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 -2.0f, 0.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 -4.0f, 1.0f, 0.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,

		 -4.0f, 0.0f, 0.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 -4.0f, 1.0f, 0.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
		 -2.0f, 0.0f, -1.0f, 0.25f, 0.5f, 0.0f, 0.0f, -1.0f,
	    //triangulos base arriba

		-4.0f, 1.0f, 0.0f,	 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.0f, 1.0f, 2.0f,	 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		 0.0f, 1.0f, -2.0f,	 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		 4.0f, 1.0f, 0.0f,	 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.0f, 1.0f, 2.0f,	 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		 0.0f, 1.0f, -2.0f,	 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		-2.0f, 1.0f, -3.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    0.0f, 1.0f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    
		2.0f, 1.0f, -3.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
	    2.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    0.0f, 1.0f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		-2.0f, 1.0f, 3.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    0.0f, 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

	    2.0f, 1.0f, 3.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
	    2.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    0.0f, 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		//base abajo
		-4.0f, 0.0f, 0.0f,	 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.0f, 0.0f, 2.0f,	 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		 0.0f, 0.0f, -2.0f,	 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		 4.0f, 0.0f, 0.0f,	 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
		 0.0f, 0.0f, 2.0f,	 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		 0.0f, 0.0f, -2.0f,	 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		-2.0f, 0.0f, -3.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    0.0f, 0.0f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    
		2.0f, 0.0f, -3.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
	    2.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    0.0f, 0.0f, -2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		-2.0f, 0.0f, 3.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
	    -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    0.0f, 0.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

	    2.0f, 0.0f, 3.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f,
	    2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	    0.0f, 0.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
	};

	MeshPrimitive *dado = new MeshPrimitive();//3*8* reng(38)     //3*reng (38)
	dado->CreateMeshPrimitive(cubo_vertices, cubo_indices, 912, 114);
	meshListPrimitive.push_back(dado);
}

void InitShaders()
{
	auto shader = new Shader();
	shader->loadShader("shaders/shader.vert", "shaders/shader.frag");
	shaders[ShaderTypes::BASE_SHADER] = shader;

	auto lightShader = new Shader();
	lightShader->loadShader("shaders/shader_light.vert", "shaders/shader_light.frag");
	shaders[ShaderTypes::LIGHT_SHADER] = lightShader;

	auto boneShader = new Shader();
	boneShader->loadShader("shaders/shader_bone.vert", "shaders/shader_light.frag");
	shaders[ShaderTypes::BONE_SHADER] = boneShader;
}

void InitCameras()
{
	cameras
	    .addCamera(new Camera::StaticCamera(glm::vec3(-134.618, 124.889, 4.39917),
	                                        glm::vec3(0.0f, 1.0f, 0.0f),
	                                        0.0f, -30.0f))
	    .addCamera(new Camera::PlayerCamera(&avatarPlayer))
	    .addCamera(new Camera::FreeCamera(glm::vec3(0.0f, 60.0f, 20.0f),
	                                      glm::vec3(0.0f, 1.0f, 0.0f),
	                                      -60.0f, 0.0f, 0.5f, 0.5f));
	activeCamera = cameras.getAcviveCamera();
}

void InitModels()
{
	models
	    .addModel(MODELS::MAQUINA_PINBALL, "assets/Models/ModeloMaquina.obj")
	    .addModel(MODELS::FLIPPER, "assets/Models/Flipper.obj")
	    .addModel(MODELS::FLIPPERH, "assets/Models/FlipperH.obj")
	    .addModel(MODELS::MARBLE, "assets/Models/canica.obj")
	    .addModel(MODELS::MAQUINA_CRISTAL, Utils::PathUtils::getModelsPath().append("/MaquinaCristal.obj"))
	    .addModel(MODELS::MARBLE, Utils::PathUtils::getModelsPath().append("/canica.obj"))
	    .addModel(MODELS::RESORTE, Utils::PathUtils::getModelsPath().append("/Resorte.obj"))
	    .addModel(MODELS::PALANCA, Utils::PathUtils::getModelsPath().append("/Lever.obj"))
	    .addModel(MODELS::TRIANGLE, Utils::PathUtils::getModelsPath().append("/Triangle.obj"))
	    .addModel(MODELS::JK_1, Utils::PathUtils::getModelsPath().append("/Marx/Base.obj"))
	    .addModel(MODELS::JK_2, Utils::PathUtils::getModelsPath().append("/Marx/Body.obj"))
	    .addModel(MODELS::JK_3, Utils::PathUtils::getModelsPath().append("/Marx/Brazo1.obj"))
	    .addModel(MODELS::JK_4, Utils::PathUtils::getModelsPath().append("/Marx/Brazo2.obj"))
	    .addModel(MODELS::JK_5, Utils::PathUtils::getModelsPath().append("/Marx/Brazo3.obj"))
	    .addModel(MODELS::JK_6, Utils::PathUtils::getModelsPath().append("/Marx/Rotor.obj"))
	    .addModel(MODELS::POD, Utils::PathUtils::getModelsPath().append("/pod.obj"))
	    .addModel(MODELS::ROBOT, Utils::PathUtils::getModelsPath().append("/sstubby.obj"))
	    .addModel(MODELS::DESTROYED_BUILDING, Utils::PathUtils::getModelsPath().append("/ExtraModels/Building.obj"))
	    .addModel(MODELS::MUNECO, Utils::PathUtils::getModelsPath().append("/muneco_Hielo.obj"))
	    .addModel(MODELS::NORA, Utils::PathUtils::getModelsPath().append("/NoraF.obj"))
	    .addModel(MODELS::PICOG, Utils::PathUtils::getModelsPath().append("/Pico_G.obj"))
	    .addModel(MODELS::PICOM, Utils::PathUtils::getModelsPath().append("/Pico_M.obj"))
	    .loadModels();
	avatar.loadModel();
}

void InitLights()
{
	Lights::LightCollectionBuilder<Lights::DirectionalLight> directionalLightsBuilder(2);
	directionalLights = directionalLightsBuilder
	                        .addLight(Lights::DirectionalLight(1.0f, 1.0f, 1.0f,
	                                                           0.5f, 0.3f,
	                                                           0.0f, -1.0f, 0.0f))
	                        .addLight(Lights::DirectionalLight(1.0f, 1.0f, 1.0f,
	                                                           0.3f, 0.3f,
	                                                           0.0f, -1.0f, 0.0f))
	                        .build();

	Lights::LightCollectionBuilder<Lights::PointLight> pointLightsBuilder(MAX_POINT_LIGHTS); // nuevo
	pointLights = pointLightsBuilder
	                  .addLight(Lights::PointLight(ToRGB(51), 1.0f, ToRGB(119),
	                                               0.8f, 0.3f,
	                                               15.5f, 60.35f, -10.0f,
	                                               1.0f, 0.05f, 0.008f))
	                  .addLight(Lights::PointLight(ToRGB(159), 1.0f, ToRGB(51),
	                                               0.8f, 0.3f,
	                                               31.0f, 64.1f, -26.0f,
	                                               1.0f, 0.05f, 0.008f))
	                  .addLight(Lights::PointLight(ToRGB(159), 1.0f, ToRGB(51),
	                                               0.8f, 0.3f,
	                                               6.5f, 59.35f, 6.0f,
	                                               1.0f, 0.05f, 0.008f))
	                  .addLight(Lights::PointLight(ToRGB(51), 1.0f, ToRGB(119),
	                                               0.8f, 0.3f,
	                                               -58.6934, 51.8151, 9.73,
	                                               1.0f, 0.05f, 0.008f))
	                  .addLight(Lights::PointLight(ToRGB(159), 1.0f, ToRGB(51),
	                                               0.8f, 0.3f,
	                                               -58.6934, 51.8151, -9.73,
	                                               1.0f, 0.05f, 0.008f))
	                  .build();
	Lights::LightCollectionBuilder<Lights::SpotLight> spotLightBuilder(MAX_SPOT_LIGHTS);
	spotLights = spotLightBuilder
	                 .addLight(Lights::SpotLight(1.0f, 1.0f, 1.0f,
	                                             0.8f, 0.3f,
	                                             5.22617, 234.113, 1.82546,
	                                             0.0f, -1.0f, 0.0f,
	                                             1.0f, 0.008f, 0.001f,
	                                             50.0f))
	                 .build();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#pragma clang diagnostic ignored "-Wreturn-type"
#pragma clang diagnostic ignored "-Wunused-parameter"

void InitAnimations()
{
	marblePreLaunch
	    .addCondition([](float dt) -> bool
	                  {
		                  if(expandeResorte >= 0.5f && Input::MouseInput::GetInstance().getCurrentKeymap()->at(GLFW_MOUSE_BUTTON_RIGHT).pressed)
		                  {
								expandeResorte -= 0.05f * dt;
			                    leverPos += leverDirection * 0.08f * dt;
								return false;
		                  }
		                  return !Input::MouseInput::GetInstance().getCurrentKeymap()->at(GLFW_MOUSE_BUTTON_RIGHT).pressed; })
	    .addCondition([](float dt) -> bool
	                  {
		                  if (expandeResorte <=0.5)
		                  {
			                  expandeResorte += 0.05f * dt;
			                  return false;
		                  }
		                  expandeResorte = 1.0f;
		                  leverPos = {-85.369f, 43.931f, 36.921f};
		                  return true; })
	    .prepare();
	PicoJerarquia1
	    .addCondition([](float delta) -> bool
	        { return activarP1; })
	    .addCondition([](float delta) -> bool
	        {
				pointLights.toggleLight(0,true);//luces se prende
		        if (rotPico1 < 720)
		        {
			        if (rotPicoZ1 < 6 && rotPico1 < 70)
			        {
				        rotPicoZ1 += 10 * escOffPico * delta;
			        }
			        rotPico1 += rotOffPico * delta;
			        if (escPico1 < 2.2)
			        {
				        escPico1 += escOffPico * delta;
			        }
			        if (rotPicoZ1 > 6 && rotPico1 > 650)
			        {
				        rotPicoZ1 -= 10 * escOffPico * delta;
			        }

			        return false;
		        }
		        else
		        {
			        rotPicoZ1 = 0;
		        }
		        return true; })
	    .prepare();

	PicoJerarquia2
	    .addCondition([](float delta) -> bool
	        { return activarP2; })
	    .addCondition([](float delta) -> bool
	        {
				pointLights.toggleLight(1,true);//luces se prende
		        if (rotPico2 < 720)
		        {
			        if (rotPicoZ2 < 6 && rotPico2 < 70)
			        {
				        rotPicoZ2 += 10 * escOffPico * delta;
			        }
			        rotPico2 += rotOffPico * delta;
			        if (escPico2 < 2.2)
			        {
				        escPico2 += escOffPico * delta;
			        }
			        if (rotPicoZ2 > 6 && rotPico2 > 650)
			        {
				        rotPicoZ2 -= 10 * escOffPico * delta;
			        }

			        return false;
		        }
		        else
		        {
			        rotPicoZ2 = 0;
		        }
		        return true; })
	    .prepare();

	PicoJerarquia3
	    .addCondition([](float delta) -> bool
	        {return activarP3;	})
	    .addCondition([](float delta) -> bool
	        {
				pointLights.toggleLight(2,true);//luces se prende
		        if (rotPico3 < 720)
		        {
			        if (rotPicoZ3 < 6 && rotPico3 < 70)
			        {
				        rotPicoZ3 += 10 * escOffPico * delta;
			        }
			        rotPico3 += rotOffPico * delta;
			        if (escPico3 < 2.2)
			        {
				        escPico3 += escOffPico * delta;
			        }
			        if (rotPicoZ3 > 6 && rotPico3 > 650)
			        {
				        rotPicoZ3 -= 10 * escOffPico * delta;
			        }

			        return false;
		        }
		        else
		        {
			        rotPicoZ3 = 0;
		        }
		        return true; })
	    .prepare();
	// animacion canica simple (version anterior)
	CanicaAS
	    .addCondition([](float delta) -> bool
	        { return activarCanicaSimple; })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC != 0) return false;
		        if (t == 0)
		        {
			        AnteriorX = -78.0, AnteriorY = 45.5, AnteriorZ = 7.5;
			        NuevoX = -77.3153076171875, NuevoY = 46.67758560180664, NuevoZ = 36.76131057739258;
			        Cx = ((1 - t) * AnteriorX + t * NuevoX);
			        Cy = ((1 - t) * AnteriorY + t * NuevoY);
			        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
			        t += 0.00005f * delta;
			        return false;
		        }
		        else if (t < 1 && t > 0)
		        {
			        Cx = ((1 - t) * AnteriorX + t * NuevoX);
			        Cy = ((1 - t) * AnteriorY + t * NuevoY);
			        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
			        t += 0.005f * delta;
			        return false;
		        }
		        t = 0;
		        flagC = 1;
		        return true;
	        })
	    .prepare();
	CanicaAS2
	    .addCondition([](float delta) -> bool
	        { return activarCanicaSimple2; })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 1)
		        {
			        if (t == 0)
			        {
				        AnteriorX = -77.3, AnteriorY = 46.6, AnteriorZ = 36.7;
				        NuevoX = -41.0, NuevoY = 49.9, NuevoZ = 36.7;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 2;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 2)
		        {
			        if (t == 0)
			        {
				        AnteriorX = -41.0, AnteriorY = 49.9, AnteriorZ = 36.7;
				        NuevoX = 21.12, NuevoY = 56.79, NuevoZ = 36.7;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 3;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 3)
		        {
			        if (t == 0)
			        {
				        AnteriorX = 21.12, AnteriorY = 56.79, AnteriorZ = 36.7;
				        NuevoX = 42.8, NuevoY = 58.9, NuevoZ = 26.6;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 4;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 4)
		        {
			        if (t == 0)
			        {
				        AnteriorX = 42.8, AnteriorY = 58.9, AnteriorZ = 26.6;
				        NuevoX = 46.2, NuevoY = 58.9, NuevoZ = 16.3;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 5;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 5)
		        {
			        if (t == 0)
			        {
				        AnteriorX = 46.2, AnteriorY = 58.9, AnteriorZ = 16.3;
				        NuevoX = 11.1, NuevoY = 55.4, NuevoZ = 4.0;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.015 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.015 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 6;
			        activarP3 = true;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        { PicoJerarquia3.start();
				return activarP3; })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 6)
		        {
			        if (t == 0)
			        {
				        AnteriorX = 11.1, AnteriorY = 55.4, AnteriorZ = 4.0;
				        NuevoX = 25.1, NuevoY = 57.3, NuevoZ = 1.0;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 7;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 7)
		        {
			        if (t == 0)
			        {
				        AnteriorX = 25.1, AnteriorY = 57.3, AnteriorZ = 1.0;
				        NuevoX = 5.0, NuevoY = 55.0, NuevoZ = -9.9;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.01 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.01 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 8;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 8)
		        {
			        if (t == 0)
			        {
				        AnteriorX = 5.2, AnteriorY = 55.1, AnteriorZ = -10.1;
				        NuevoX = 26.3, NuevoY = 57.4, NuevoZ = -23.5;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 9;
			        activarP2 = true;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        { PicoJerarquia2.start();
				return activarP2; })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 9)
		        {
			        if (t == 0)
			        {
				        AnteriorX = 26.3, AnteriorY = 57.4, AnteriorZ = -23.5;
				        NuevoX = 21.3, NuevoY = 56.9, NuevoZ = -3.5;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.025 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.025 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 10;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 10)
		        {
			        if (t == 0)
			        {
				        AnteriorX = 21.3, AnteriorY = 56.9, AnteriorZ = -3.5;
				        NuevoX = -40.3, NuevoY = 50.5, NuevoZ = 9.9;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.015 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.015 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 11;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 11)
		        {
			        if (t == 0)
			        {
				        AnteriorX = -40.3, AnteriorY = 50.5, AnteriorZ = 9.9;
				        NuevoX = -52.0, NuevoY = 49.25, NuevoZ = -10.3;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 12;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 12)
		        {
			        if (t == 0)
			        {
				        AnteriorX = -52.0, AnteriorY = 49.25, AnteriorZ = -10.3;
				        NuevoX = -32.3, NuevoY = 51.2, NuevoZ = -5.5;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.02 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 13;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 13)
		        {
			        if (t == 0)
			        {
				        AnteriorX = -32.3, AnteriorY = 51.2, AnteriorZ = -5.5;
				        NuevoX = -78.0, NuevoY = 46.3, NuevoZ = -5.5;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.015 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.015 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 14;
			        return true;
		        }
	        })
	    .addCondition([](float delta) -> bool
	        {
		        if (flagC == 14)
		        {
			        if (t == 0)
			        {
				        AnteriorX = -78.0, AnteriorY = 46.3, AnteriorZ = -5.5;
				        NuevoX = -78.0, NuevoY = 46.3, NuevoZ = 7.5;
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.01 * delta;
				        return false;
			        }
			        else if (t < 1 && t > 0)
			        {
				        Cx = ((1 - t) * AnteriorX + t * NuevoX);
				        Cy = ((1 - t) * AnteriorY + t * NuevoY);
				        Cz = ((1 - t) * AnteriorZ + t * NuevoZ);
				        t += 0.01 * delta;
				        return false;
			        }
			        t = 0;
			        flagC = 0;
			        activarCanicaSimple = false;
			        return true;
		        }
	        })
	    .prepare();

	MjPos_0 = {0.0f, 2.9f, -3.2f};
}

#pragma clang diagnostic pop

void InitPlayers()
{
	avatarPlayer.setPosition({-21.7661, 50.5184, -10.7455});
}

void updateFlippers()
{
	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_RIGHT_SHIFT).pressed)
	{
		if (rightFlipperRotation > -20)
			rightFlipperRotation -= 5;
	}
	else
	{
		if (rightFlipperRotation < 20)
			rightFlipperRotation += 5;
	}

	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_LEFT_SHIFT).pressed)
	{
		if (leftFlipperRotation < 20)
			leftFlipperRotation += 5;
	}
	else
	{
		if (leftFlipperRotation > -20)
			leftFlipperRotation -= 5;
	}

	if (Input::KeyboardInput::GetInstance().getCurrentKeymap()->at(GLFW_KEY_M).pressed)
	{
		if (upFlipperRotation < 20)
			upFlipperRotation += 5;
	}
	else
	{
		if (upFlipperRotation > -20)
			upFlipperRotation -= 5;
	}
}

void LoadAnimations()
{
	marbleKfAnim.loadFromFile(MARBLE_ANIM_FILE);
}

void exitProgram()
{
	for (auto &shader : shaders)
		delete shader.second;
}

void ChangeShader(Shader *shader)
{
	shader->useProgram();
	uModel = shader->getUniformModel();
	uProjection = shader->getUniformProjection();
	uView = shader->getUniformView();
	uEyePosition = shader->getUniformEyePosition();
	uColor = shader->getUniformColor();
	uTexOffset = shader->getUniformTextureOffset();
	uSpecularIntensity = shader->getUniformSpecularIntensity();
	uShininess = shader->getUniformShininess();
}

float CalculateAvatarHeight()
{
	// Punto inferior -78.9389, 44.4252, 20.9459
	// mov            54.7505, 44.4252, 20.9459
	float avatarPosX = avatarPlayer.getPosition().x;
	if (avatarPosX < -80) return 44.4252f;
	auto cat = (float) std::abs(avatarPosX + 78.9389);
	float alturaFinal = std::sin(glm::radians(6.0f)) * cat;
	return 44.4252f + alturaFinal;
}

void CheckAvatarLimits()
{
	// limites inferiores = -79.7698, 46.4216, 38.4736
	// limites superiores = 53.3977, 75.7373, -38.3946
	auto avp = avatarPlayer.getPosition();
	if (avp.x < -80)
		avatarPlayer.setPosition({-80, avp.y, avp.z});
	if (avp.x > 54)
		avatarPlayer.setPosition({54, avp.y, avp.z});
	if (avp.z < -38)
		avatarPlayer.setPosition({avp.x, avp.y, -38});
	if (avp.z > 38)
		avatarPlayer.setPosition({avp.x, avp.y, 38});
}

void InitAudios()
{
	mblKeyframesAudio.Init();
	mblSaAudio.Init();
	walkingAudio.Init();
}

int main(int argc, char **argv)
{
	mainWindow = Window(1280, 720, "Proyecto Final \"Maquina de pinball\" - Semestre 2024-1");

	if (!mainWindow.Init())
	{
		std::cerr << "No se pudo iniciar la ventana\n";
		return 1;
	}
	CrearPrimitiva();
	// Inicializar los componentes del programa
	Audio::AudioDevice::GetInstance(); // inicializa el componente de audio
	Audio::AudioDevice::InitAlut(&argc, argv);
	InitShaders();
	InitKeymaps();
	InitCameras();
	InitModels();
	InitLights();
	InitPlayers();
	InitAnimations();
	LoadAnimations();
	InitAudios();
	primitiva = Model::Texture("assets/Textures/Hielo3.png");
	primitiva.LoadTexture();


	// region Skybox settings
	// SKyBoxes Faces Day
	std::vector<std::string> skbfDay;
	// SKyBoxes Faces Night
	std::vector<std::string> skbfNight;

	// Comando para rotar las texturas: Get-ChildItem -Recurse -Filter "*.png" | foreach { if ($_.Name -match '\wy.png') { magick.exe $_.Name -rotate 180 $_.Name  } }
	// Pasar de panorama a cubica: https://jaxry.github.io/panorama-to-cubemap/
	skbfDay.emplace_back("assets/Textures/Skybox/Day/nx.png");
	skbfDay.emplace_back("assets/Textures/Skybox/Day/px.png");
	skbfDay.emplace_back("assets/Textures/Skybox/Day/ny.png");
	skbfDay.emplace_back("assets/Textures/Skybox/Day/py.png");
	skbfDay.emplace_back("assets/Textures/Skybox/Day/nz.png");
	skbfDay.emplace_back("assets/Textures/Skybox/Day/pz.png");
	skyboxDay = Skybox(skbfDay);

	skbfNight.emplace_back("assets/Textures/Skybox/Night/nx.png");
	skbfNight.emplace_back("assets/Textures/Skybox/Night/px.png");
	skbfNight.emplace_back("assets/Textures/Skybox/Night/ny.png");
	skbfNight.emplace_back("assets/Textures/Skybox/Night/py.png");
	skbfNight.emplace_back("assets/Textures/Skybox/Night/nz.png");
	skbfNight.emplace_back("assets/Textures/Skybox/Night/pz.png");
	skyboxNight = Skybox(skbfNight);

	skyBoxCurrent = &skyboxDay;
	// endregion

	matMetal = Model::Material(256.0f, 256);
	Material_brillante = Model::Material(4.0f, 256);
	Material_opaco = Model::Material(0.3f, 4);

	Utils::ModelMatrix handler(glm::mat4(1.0f));
	glm::mat4 model(1.0f);
	glm::mat4 modelaux(1.0f);
	glm::mat4 modelaux2(1.0f);
	glm::mat4 modelaux3(1.0f);
	glm::vec3 color(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	// modelos
	auto maquinaPinball = models[MODELS::MAQUINA_PINBALL];
	auto cristal = models[MODELS::MAQUINA_CRISTAL];
	auto flipper = models[MODELS::FLIPPER];
	auto flipperh = models[MODELS::FLIPPERH];
	auto marbleKf = models[MODELS::MARBLE];
	auto resorte = models[MODELS::RESORTE];
	auto lever = models[MODELS::PALANCA];
	auto triangle = models[MODELS::TRIANGLE];
	auto Nora = models[MODELS::NORA];
	auto PicoG = models[MODELS::PICOG];
	auto PicoM = models[MODELS::PICOM];
	auto Muneco = models[MODELS::MUNECO];
	auto destroyedBuilding = models[MODELS::DESTROYED_BUILDING];
	auto robot = models[MODELS::ROBOT];
	auto pod = models[MODELS::POD];

	Animation::BoneAnimation walkAnimation(Utils::PathUtils::getModelsPath().append("/2b_walk_static.fbx"), &avatar);
	Animation::BoneAnimation idleAnimation(Utils::PathUtils::getModelsPath().append("/2b_idle.fbx"), &avatar);
	Animation::BoneAnimator avatarAnimator(&idleAnimation);
	avatarAnimator.PlayAnimation(&walkAnimation);
	avatarPlayer.setEnableControls(false); // La primer cámara es la libre, el avatar debe estar desactivado.

	// Shaders
	auto shaderLight = shaders[ShaderTypes::BONE_SHADER];

	while (!mainWindow.shouldClose())
	{
		auto now = (float) glfwGetTime();
		deltaTime = now - lastTime;
		deltaTimeAnim = deltaTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		activeCamera->KeyControl(Input::KeyboardInput::GetInstance());
		activeCamera->FixedUpdate();

		avatarPlayer.Move();
		auto avPos = avatarPlayer.getPosition();
		avatarPlayer.setPosition({avPos.x, CalculateAvatarHeight(), avPos.z});
		CheckAvatarLimits();

		if (avatarPlayer.isMoving())
		{
			avatarAnimator.PlayAnimation(&walkAnimation);
			walkingAudio.Update(avatarPlayer.getPosition());
			walkingAudio.Play();
		}
		else
		{
			avatarAnimator.PlayAnimation(&idleAnimation);
			walkingAudio.Stop();
		}

		avatarAnimator.UpdateAnimation(deltaTimeAnim);

		// update anim
		updateFlippers();
		marblePreLaunch.update(deltaTime);

		PicoJerarquia1.update(deltaTime);
		PicoJerarquia2.update(deltaTime);
		PicoJerarquia3.update(deltaTime);
		CanicaAS.update(deltaTime);
		CanicaAS2.update(deltaTime);
		canicaASPos = {Cx, Cy, Cz};

		// Audio update
		Audio::AudioController::UpdateListenerPosition(activeCamera->getCameraPosition());
		Audio::AudioController::UpdateListenerOrientation(activeCamera->getCameraDirection(), activeCamera->getUp());
		mblKeyframesAudio.Update(marbleKfAnim.getPosition());

		if (timer <= glfwGetTime())
		{
			if (ambLight == AMB_LIGHTS::DAY)
			{
				skyBoxCurrent = &skyboxNight;
				ambLight = AMB_LIGHTS::NIGHT;
			}
			else
			{
				skyBoxCurrent = &skyboxDay;
				ambLight = AMB_LIGHTS::DAY;
			}
			timer = (float) glfwGetTime() + 8;
		}

		if (captureMode)
			marbleEntity.update(nullptr, deltaTime);
		else
		{
			if (marbleKfAnim.isPlaying())
			{
				marbleKfAnim.play();
				mblKeyframesAudio.Play();
			}
			else
			{
				marbleKfAnim.resetAnimation();
				mblKeyframesAudio.Stop();
			}
		}

		if (CanicaAS2.isStarted())
			mblSaAudio.Play();
		else
			mblSaAudio.Stop();

		// region Shader settings
		// Configuración del shader
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyBoxCurrent->DrawSkybox(activeCamera->calculateViewMatrix(), mainWindow.getProjectionMatrix());
		ChangeShader(shaders[ShaderTypes::BONE_SHADER]);

		// Camara
		glUniformMatrix4fv((GLint) uProjection, 1, GL_FALSE, glm::value_ptr(mainWindow.getProjectionMatrix()));
		glUniformMatrix4fv((GLint) uView, 1, GL_FALSE, glm::value_ptr(activeCamera->calculateViewMatrix()));
		auto camPos = activeCamera->getCameraPosition();
		glUniform3f((GLint) uEyePosition, camPos.x, camPos.y, camPos.z);

		// Iluminacion
		shaderLight->SetDirectionalLight(&directionalLights[ambLight]);
		shaderLight->SetSpotLights(spotLights.getLightArray(), spotLights.getCurrentCount());
		shaderLight->SetPointLights(pointLights.getLightArray(), pointLights.getCurrentCount());

		if (marbleKfAnim.Returnindex() > 6)
		{
			activarP3 = true;
			PicoJerarquia3.start();
		}
		if (marbleKfAnim.Returnindex() > 12)
		{
			activarP1 = true;
			PicoJerarquia1.start();
		}

		if (!activarP1)
		{
			pointLights.toggleLight(0, false); // luces apagadas de los picos
		}
		if (!activarP2)
		{
			pointLights.toggleLight(1, false); // luces apagadas de los picos
		}
		if (!activarP3)
		{
			pointLights.toggleLight(2, false);
		} /////////////////////////////

		toffset = {0.0f, 0.0f};
		color = {1.0f, 1.0f, 1.0f};
		Material_opaco.UseMaterial(uSpecularIntensity, uShininess);

		glUniform2fv((GLint) uTexOffset, 1, glm::value_ptr(toffset));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));
		// endregion Shader settings

		// Para tomar las coordenadas de Blender
		// y <-> z
		// z -> -z
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(0.0, 0.0, 0.0)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));
		maquinaPinball.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(leverPos)
		            .rotateZ(110)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		lever.render();

		// region Flippers
		// Fliper izquierdo
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-58, 48, 10)
		            .rotateZ(6)
		            .rotateY(rightFlipperRotation)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper.render();
		// Flipper derecho
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-58, 48, -19)
		            .rotateZ(6)
		            .rotateY(180 + leftFlipperRotation)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper.render();
		// Flipper superior
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(1.637, 54, -13.314)
		            .rotateZ(6)
		            .rotateY(180 + upFlipperRotation)
		            .scale(0.586)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipperh.render();
		// endregion Flippers

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-43.281, 51.479, -22.837)
		            .rotateZ(6)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		triangle.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-43.281, 51.479, 14.002)
		            .rotateZ(6)
		            .scale(1.0f, 1.0f, -1.0f)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		triangle.render();

		// region Modelo Jerarquico 1 (art 0-3)
		// Para tomar las coordenadas de Blender
		// y <-> z
		// z -> -z
		// 1er pico animado
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(15.5, 58.35, -10)
		            .rotateY(rotPico1)
		            .saveActualState(modelaux) // modelaux = model;
		            .scale(2.2)
		            .rotateZ(6 - rotPicoZ1)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoG.render();
		// 1
		model = handler.setMatrix(modelaux)
		            .translate(-2.5, 1.55, 0)
		            .scale(escPico1)
		            .rotateZ(44.39)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 2
		model = handler.setMatrix(modelaux)
		            .translate(.5, 3.55, +2)
		            .rotateZ(-8)
		            .rotateX(48)
		            .scale(escPico1)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 3
		model = handler.setMatrix(modelaux)
		            .translate(.5, 3.55, -2)
		            .rotateZ(-8)
		            .rotateX(-48)
		            .scale(escPico1)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 4
		model = handler.setMatrix(modelaux)
		            .translate(-0.7, 5.55, +.5)
		            .scale(escPico1)
		            .rotateZ(40)
		            .rotateX(24)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 5
		model = handler.setMatrix(modelaux)
		            .translate(-0.5, 5.55, -.5)
		            .scale(escPico1)
		            .rotateZ(25)
		            .rotateX(-20)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 2do pico
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(31, 62.1, -26)
		            .rotateY(rotPico2)
		            .saveActualState(modelaux2) // modelaux = model;
		            .scale(2.2)
		            .rotateZ(6 - rotPicoZ2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoG.render();
		// 1
		model = handler.setMatrix(modelaux2)
		            .translate(-2.5, 1.55, 0)
		            .scale(escPico2)
		            .rotateZ(44.39)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 2
		model = handler.setMatrix(modelaux2)
		            .translate(.5, 3.55, +2)
		            .rotateZ(-8)
		            .rotateX(48)
		            .scale(escPico2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 3
		model = handler.setMatrix(modelaux2)
		            .translate(.5, 3.55, -2)
		            .rotateZ(-8)
		            .rotateX(-48)
		            .scale(escPico2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 4
		model = handler.setMatrix(modelaux2)
		            .translate(-0.7, 5.55, +.5)
		            .scale(escPico2)
		            .rotateZ(40)
		            .rotateX(24)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 5
		model = handler.setMatrix(modelaux2)
		            .translate(-0.5, 5.55, -.5)
		            .scale(escPico2)
		            .rotateZ(25)
		            .rotateX(-20)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// endregion

		// 3do pico
		// region Extra models

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(6.5, 57.35, 6)
		            .rotateY(rotPico3)
		            .saveActualState(modelaux3) // modelaux = model;
		            .scale(2.2)
		            .rotateZ(6 - rotPicoZ3)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoG.render();
		// 1
		model = handler.setMatrix(modelaux3)
		            .translate(-2.5, 1.55, 0)
		            .scale(escPico3)
		            .rotateZ(44.39)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));

		PicoM.render();
		// 2
		model = handler.setMatrix(modelaux3)
		            .translate(.5, 3.55, +2)
		            .rotateZ(-8)
		            .rotateX(48)
		            .scale(escPico3)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 3
		model = handler.setMatrix(modelaux3)
		            .translate(.5, 3.55, -2)
		            .rotateZ(-8)
		            .rotateX(-48)
		            .scale(escPico3)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 4
		model = handler.setMatrix(modelaux3)
		            .translate(-0.7, 5.55, +.5)
		            .scale(escPico3)
		            .rotateZ(40)
		            .rotateX(24)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 5
		model = handler.setMatrix(modelaux3)
		            .translate(-0.5, 5.55, -.5)
		            .scale(escPico3)
		            .rotateZ(25)
		            .rotateX(-20)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// endregion

		// region MODELOS_NIER
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(53.983, 71.612, -27.538)
		            .rotateY(46.061)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		destroyedBuilding.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(56.002, 71.377, 35.124)
		            .rotateY(-416.5)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		destroyedBuilding.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(44.797, 66.892, 0)
		            .rotateY(180)
		            .scale(11.492)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		robot.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-1.48956, 66.183f + (float) (1.5f * sin(glfwGetTime())), 19.9368)
		            .rotateY(131.41)
		            .scale(2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		pod.render();
		// endregion

		// region Entity Marble
		matMetal.UseMaterial(uSpecularIntensity, uShininess);
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(captureMode ? marbleEntity.getPosition() : marbleKfAnim.getPosition() + marbleKfAnim.getMovement())
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		marbleKf.render();

		matMetal.UseMaterial(uSpecularIntensity, uShininess);
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(canicaASPos)
		            //.rotateX(rotXCanica)
		            //.rotateZ(6 + rotZCanica)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		marbleKf.render();
		// endregion Entity Marble

		// region Resorte

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-83.614f, 45.123f, 36.931f)
		            .rotateZ(-76)
		            .scale(0.582)
		            .scale(1.0f, expandeResorte, 1.0f)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		resorte.render();

		// endregion Resorte

		// region muñeco de hielo
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(8, 55, -26)
		            .scale(0.4)
		            .rotateZ(-6)
		            .rotateY(180)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		Muneco.render();
		// endregion muñeco de nieve

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(0, -80, 0)
		            .scale(60,10,60)
		            //.rotateZ(-6)
		            //.rotateY(180)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		primitiva.UseTexture();
		meshListPrimitive[0]->RenderMeshPrimitive();

		// region ALPHA
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// region AVATAR
		Material_opaco.UseMaterial(uSpecularIntensity, uShininess);
		auto transforms = avatarAnimator.GetFinalBoneMatrices();
		for (int i = 0; i < (int) transforms.size(); i++)
			shaderLight->setMat4((boost::format("finalBonesMatrices[%d]") % i).str(), transforms[i]);

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(avatarPlayer.getPosition())
		            .rotateY(avatarPlayer.getRotation().y)
		            .scale(0.03)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		avatar.render();
		// endregion

		// region Nora
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(26, 61, 19)
		            .scale(2.0)
		            .rotateY(-90)
		            .rotateX(6)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		Nora.render();
		// endregion

		// region Cristal
		model = glm::mat4(1.0f);
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		cristal.render();
		glDisable(GL_BLEND);
		// endregion
		// endregion ALPHA

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	Audio::AudioDevice::Terminate();
	exitProgram();

	return 0;
}

#pragma clang diagnostic pop