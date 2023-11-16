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
#include "Animation/KeyFrameAnimation.h"
#include "Audio/AudioDevice.h"
#include "Entity/SimpleEntity.h"
#include "GlobalConstants.h"
#include "Lights/DirectionalLight.h"
#include "Lights/LightCollection.h"
#include "Skybox.h"
#include "Utils/ModelMatrix.h"
#include "Window.h"
#include "camera/Camera.h"
#include "camera/CameraCollection.h"
#include "input/KeyboardInput.h"
#include "model/BoneModel.h"
#include "model/Material.h"
#include "model/ModelCollection.h"

// region Global Variables
Window mainWindow;
Camera::CameraCollection cameras;
Camera::Camera *activeCamera;
Model::ModelCollection models;
Lights::LightCollection<Lights::DirectionalLight> directionalLights;
Lights::LightCollection<Lights::PointLight> pointLights;
Lights::LightCollection<Lights::SpotLight> spotLights;
Skybox skyboxDay;
Skybox skyboxNight;
Skybox *skyBoxCurrent;
Entity::SimpleEntity marbleEntity;
Animation::KeyFrameAnimation marbleKfAnim;
Animation::Animation marbleAnimation;
Animation::Animation marblePreLaunch;
Animation::Animation marblePostLaunch;
glm::vec3 marblePos;
glm::vec3 leverPos = {-85.369f, 43.931f, 36.921f};
const glm::vec3 leverEnd = {-90.089f, 42.213f, 36.921f};
const glm::vec3 leverDirection = glm::normalize(leverEnd - leverPos);
// const float movLeverDistance = glm::length(leverEnd - leverPos);
Model::BoneModel avatar(Utils::PathUtils::getModelsPath().append("/2b.obj"));

Model::Material matMetal;
Model::Material Material_brillante;
Model::Material Material_opaco;

float timer = 0.0f;

std::unordered_map<int, Shader *> shaders;
Animation::Animation PicoJerarquia1;
Animation::Animation PicoJerarquia2;
Animation::Animation PicoJerarquia3;
//Animation::Animation modeloJerarquico1;
bool mJ1_trigger = false;

// Posicion numeros: 60.3627, 115.599, -34.7832

float deltaTime = 0.0f;
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

bool activarP1 = false, activarP2 = false, activarP3 =false;
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

// MJ_BASE_ROT(1)
// MJ_BASE_ROT(2)

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
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_I,
	        []() -> void
	        {
		        activarP1 = true;
		        PicoJerarquia1.start();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_O,
	        []() -> void
	        {
		        activarP2 = true;
		        PicoJerarquia2.start();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_P,
	        []() -> void
	        {
		        activarP3 = true;
		        PicoJerarquia3.start();
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
		        spotLights.toggleLight(1, !spotLights.getLightStatus(1));
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_2,
	        []() -> void
	        {
		        spotLights.toggleLight(2, !spotLights.getLightStatus(2));
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_3,
	        []() -> void
	        {
		        pointLights.toggleLight(0, !pointLights.getLightStatus(0));
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_4,
	        []() -> void
	        {
		        pointLights.toggleLight(1, !pointLights.getLightStatus(1));
	        });
	/*
	Input::KeyboardInput::GetInstance()
	    .createKeymap(KEYMAPS::KEYFRAME_CAPTURE)
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_R,
	        []() -> void
	        {
		        activarP1 = 1;
				//activeCamera = cameras.switchCamera();
	        });*/

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
		        if (!captureMode)
			        marbleKfAnim.play();
	        })
	    .addMoveCallback(
	        KEYMAPS::FREE_CAMERA,
	        [](float) -> void
	        {
		        activeCamera->mouseControl(Input::MouseInput::GetInstance());
	        });
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
	cameras.addCamera(new Camera::Camera(glm::vec3(0.0f, 60.0f, 20.0f),
	                                     glm::vec3(0.0f, 1.0f, 0.0f),
	                                     -60.0f, 0.0f, 0.5f, 0.5f));
	cameras.addCamera(new Camera::Camera(glm::vec3(-134.618, 124.889, 4.39917),
	                                     glm::vec3(0.0f, 1.0f, 0.0f),
	                                     0.0f, -30.0f, 0.5f, 0.5f, true));
	activeCamera = cameras.getAcviveCamera();
}

void InitModels()
{
	models
	    .addModel(MODELS::MAQUINA_PINBALL, "assets/Models/pinballFRIO.obj")
	    .addModel(MODELS::FLIPPER, "assets/Models/FlipperH.obj")
	    .addModel(MODELS::MARBLE, "assets/Models/canica.obj")
	    .addModel(MODELS::MAQUINA_CRISTAL, Utils::PathUtils::getModelsPath().append("/MaquinaCristal.obj"))
	    .addModel(MODELS::JK_1, Utils::PathUtils::getModelsPath().append("/Marx/Base.obj"))
	    .addModel(MODELS::JK_2, Utils::PathUtils::getModelsPath().append("/Marx/Body.obj"))
	    .addModel(MODELS::JK_3, Utils::PathUtils::getModelsPath().append("/Marx/Brazo1.obj"))
	    .addModel(MODELS::JK_4, Utils::PathUtils::getModelsPath().append("/Marx/Brazo2.obj"))
	    .addModel(MODELS::JK_5, Utils::PathUtils::getModelsPath().append("/Marx/Brazo3.obj"))
	    .addModel(MODELS::JK_6, Utils::PathUtils::getModelsPath().append("/Marx/Rotor.obj"))
	    .addModel(MODELS::MARBLE, Utils::PathUtils::getModelsPath().append("/canica.obj"))
	    .addModel(MODELS::RESORTE, Utils::PathUtils::getModelsPath().append("/Resorte.obj"))
	    .addModel(MODELS::PALANCA, Utils::PathUtils::getModelsPath().append("/Lever.obj"))
	    .addModel(MODELS::ROBOT, Utils::PathUtils::getModelsPath().append("/sstubby.obj"))
	    .addModel(MODELS::TRIANGLE, Utils::PathUtils::getModelsPath().append("/TriangleH.obj"))
	    .addModel(MODELS::POD, Utils::PathUtils::getModelsPath().append("/pod.obj"))
		.addModel(MODELS::MUNECO,  Utils::PathUtils::getModelsPath().append("/muneco_Hielo.obj"))
		.addModel(MODELS::NORA, Utils::PathUtils::getModelsPath().append( "/NoraF.obj"))
		.addModel(MODELS::PICOG, Utils::PathUtils::getModelsPath().append( "/Pico_G.obj"))
	    .addModel(MODELS::PICOM, Utils::PathUtils::getModelsPath().append("/Pico_M.obj"))
	    //.addModel(MODELS::DESTROYED_BUILDING, Utils::PathUtils::getModelsPath().append("/ExtraModels/Building.obj"))
	    .loadModels();
#ifdef AVATAR
	avatar.loadModel();
#endif
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

	Lights::LightCollectionBuilder<Lights::PointLight> pointLightsBuilder(5);
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
	Lights::LightCollectionBuilder<Lights::SpotLight> spotLightBuilder(3);
	spotLights = spotLightBuilder
	                 .addLight(Lights::SpotLight(1.0f, 1.0f, 1.0f,
	                                             0.8f, 0.3f,
	                                             5.22617, 234.113, 1.82546,
	                                             0.0f, -1.0f, 0.0f,
	                                             1.0f, 0.008f, 0.001f,
	                                             50.0f))
	                 /*.addLight(Lights::SpotLight(ToRGB(199), 1.0f, ToRGB(51),
	                                             0.8f, 0.3f,
	                                             47.6584, 84.0895, 36.4503,
	                                             -2.0f, -2.0f, -2.0f,
	                                             1.0f, 0.005f, 0.0008f,
	                                             20.0f))
	                 .addLight(Lights::SpotLight(1.0f, ToRGB(221), ToRGB(51),
	                                             0.8f, 0.3f,
	                                             47.6584, 84.0895, -36.4503,
	                                             -2.0f, -2.0f, 2.0f,
	                                             1.0f, 0.005f, 0.0008f,
	                                             20.0f))*/
	                 .build();
}

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
	    .addCondition(
	        [](float delta) -> bool
	        { return activarP1; })
	    .addCondition(
	        [](float delta) -> bool
	        {
				pointLights.toggleLight(0,true);//luces se prende
				if (rotPico1 < 720)
			    {
			        if (rotPicoZ1 < 6 && rotPico1<70)
			        {
				        rotPicoZ1 +=10* escOffPico * delta;
					}
				    rotPico1 += rotOffPico * delta;
					if (escPico1 < 2.2) {
				        escPico1 += escOffPico * delta;
					}
			        if (rotPicoZ1 > 6 && rotPico1 > 650)
			        {
				        rotPicoZ1 -= 10 * escOffPico * delta;
			        }

				    return false;
			    }
			    else
			        rotPicoZ1 = 0;
				    return true; })
	    .prepare();

	PicoJerarquia2
	    .addCondition(
	        [](float delta) -> bool
	        { return activarP2; })
	    .addCondition(
	        [](float delta) -> bool
	        {
				pointLights.toggleLight(1,true);//luces se prende
				if (rotPico2 < 720)
			    {
			        std::cout << "entra";
					if (rotPicoZ2 < 6 && rotPico2<70)
			        {
				        rotPicoZ2 +=10* escOffPico * delta;
					}
				    rotPico2 += rotOffPico * delta;
					if (escPico2 < 2.2) {
				        escPico2 += escOffPico * delta;
					}
			        if (rotPicoZ2 > 6 && rotPico2 > 650)
			        {
				        rotPicoZ2 -= 10 * escOffPico * delta;
			        }

				    return false;
			    }
			    else
			        rotPicoZ2 = 0;
				    return true; })
	    .prepare();

	PicoJerarquia3
	    .addCondition(
	        [](float delta) -> bool
	        {return activarP3;	})
	    .addCondition(
	        [](float delta) -> bool
	        {
				pointLights.toggleLight(2,true);//luces se prende
				if (rotPico3 < 720)
			    {
			        if (rotPicoZ3 < 6 && rotPico3<70)
			        {
				        rotPicoZ3 +=10* escOffPico * delta;
					}
				    rotPico3 += rotOffPico * delta;
					if (escPico3 < 2.2) {
				        escPico3 += escOffPico * delta;
					}
			        if (rotPicoZ3 > 6 && rotPico3 > 650)
			        {
				        rotPicoZ3 -= 10 * escOffPico * delta;
			        }

				    return false;
			    }
			    else
			        rotPicoZ3 = 0;
				    return true; })
	    .prepare();
	//
	MjPos_0 = {0.0f, 2.9f, -3.2f};
	//	modeloJerarquico1
	//	    .addCondition([](float) -> bool
	//	                               {
	//		                  MjRot_0 = -90;
	//	                  })
	//	    .prepare();
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

int main()
{
	mainWindow = Window(1280, 720, "Proyecto Final \"Maquina de pinball\" - Semestre 2024-1");

	if (!mainWindow.Init())
	{
		std::cerr << "No se pudo iniciar la ventana\n";
		return 1;
	}

	// Inicializar los componentes del programa
	Audio::AudioDevice::GetInstance(); // inicializa el componente de audio
	InitShaders();
	InitKeymaps();
	InitCameras();
	InitModels();
	InitLights();
	InitAnimations();
	LoadAnimations();

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

	// Constantes para uniforms
	GLuint uProjection, uModel, uView, uEyePosition, uSpecularIntensity, uShininess, uTexOffset, uColor;

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
	auto marbleKf = models[MODELS::MARBLE];
	auto resorte = models[MODELS::RESORTE];
	auto lever = models[MODELS::PALANCA];
	auto triangle = models[MODELS::TRIANGLE];
	auto Nora = models[MODELS::NORA];
	auto PicoG = models[MODELS::PICOG];
	auto PicoM = models[MODELS::PICOM];
	auto Muneco = models[MODELS::MUNECO];

	// Shaders
	auto shaderLight = shaders[ShaderTypes::LIGHT_SHADER];

	while (!mainWindow.shouldClose())
	{
		auto now = (float) glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		activeCamera->keyControl(Input::KeyboardInput::GetInstance(), deltaTime);

		updateFlippers();
		marblePreLaunch.update(deltaTime);
		PicoJerarquia1.update(deltaTime);
		PicoJerarquia2.update(deltaTime);
		PicoJerarquia3.update(deltaTime);
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
			timer = (float) glfwGetTime() + 5;
		}

		if (captureMode)
			marbleEntity.update(nullptr, deltaTime);
		else
		{
			if (marbleKfAnim.isPlaying())
				marbleKfAnim.play();
			else
				marbleKfAnim.resetAnimation();
		}

		// region Shader settings
		// Configuración del shader
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skyBoxCurrent->DrawSkybox(activeCamera->calculateViewMatrix(), mainWindow.getProjectionMatrix());
		shaderLight->useProgram();
		uModel = shaderLight->getUniformModel();
		uProjection = shaderLight->getUniformProjection();
		uView = shaderLight->getUniformView();
		uEyePosition = shaderLight->getUniformEyePosition();
		uColor = shaderLight->getUniformColor();
		uTexOffset = shaderLight->getUniformTextureOffset();
		uSpecularIntensity = shaderLight->getUniformSpecularIntensity();
		uShininess = shaderLight->getUniformShininess();

		// Camara
		glUniformMatrix4fv((GLint) uProjection, 1, GL_FALSE, glm::value_ptr(mainWindow.getProjectionMatrix()));
		glUniformMatrix4fv((GLint) uView, 1, GL_FALSE, glm::value_ptr(activeCamera->calculateViewMatrix()));
		auto camPos = activeCamera->getCameraPosition();
		glUniform3f((GLint) uEyePosition, camPos.x, camPos.y, camPos.z);

		// Iluminacion
		shaderLight->SetDirectionalLight(&directionalLights[ambLight]);
		shaderLight->SetSpotLights(spotLights.getLightArray(), spotLights.getCurrentCount());
		shaderLight->SetPointLights(pointLights.getLightArray(), pointLights.getCurrentCount());

		if (activarP1 == false)
		{
			pointLights.toggleLight(0, false); // luces apagadas de los picos
		}
		if (activarP2 == false)
		{
			pointLights.toggleLight(1, false); // luces apagadas de los picos
		}
		if (activarP3 == false)
		{
			pointLights.toggleLight(2, false);
		}
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
		flipper.render();
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

		// 3do pico
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

		// region Entity Marble
		matMetal.UseMaterial(uSpecularIntensity, uShininess);
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(captureMode ? marbleEntity.getPosition() : marbleKfAnim.getPosition() + marbleKfAnim.getMovement())
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		marbleKf.render();
		// endregion Entity Marble
		//if ()
		
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
		
		//region muñeco de hielo
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(8, 55, -26)
		            .scale(0.4)
		            .rotateZ(-6)
		            .rotateY(180)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		Muneco.render();
		//endregion muñeco de nieve

		// region ALPHA
#ifdef AVATAR
		shaders[ShaderTypes::BONE_SHADER]->useProgram();
		model = handler.setMatrix(glm::mat4(1.0f))
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		avatar.render();
#endif

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
		glDisable(GL_BLEND);
		// endregion
		
		// region Cristal
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0f);
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		cristal.render();
		glDisable(GL_BLEND);
		// endregion

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	Audio::AudioDevice::Terminate();
	exitProgram();

	return 0;
}
#pragma clang diagnostic pop