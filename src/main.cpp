#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Audio/AudioDevice.h"
#include "GlobalConstants.h"
#include "Lights/DirectionalLight.h"
#include "Lights/LightCollection.h"
#include "Skybox.h"
#include "Utils/ModelMatrix.h"
#include "Window.h"
#include "camera/Camera.h"
#include "camera/CameraCollection.h"
#include "input/KeyboardInput.h"
#include "model/BasicPrimitives.h"
#include "model/Material.h"
#include "model/Model.h"
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

Model::Material Material_brillante;
Model::Material Material_opaco;

std::unordered_map<int, Shader *> shaders;

float deltaTime = 0.0f;
float lastTime = 0.0f;
const float limitFPS = 1.0f / 60.0f;

AMB_LIGHTS ambLight = AMB_LIGHTS::DAY;
// endregion

// region Variables auxiliares que no se encapsularon en otras clases :v
float rightFlipperRotation = 0.0f;
float leftFlipperRotation = 0.0f;
float upFlipperRotation = 0.0f;
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
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_T,
	        []() -> void
	        {
		        std::cout << "Mouse disabled!";
		        Input::MouseInput::GetInstance().toggleMouseEnabled();
		        mainWindow.toggleMouse();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_P,
	        []() -> void
	        {
		        std::cout << "Tecla P presionada\n";
	        },
	        true)
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_L,
	        []() -> void
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
	        });
	//	    .addCallback(
	//	        KEYMAPS::FREE_CAMERA,
	//	        GLFW_KEY_L,
	//	        []() -> void
	//	        {
	//		        std::cout << "Flipper Right: " << rightFlipperRotation << '\n';
	//		        if (rightFlipperRotation < -25)
	//			        return;
	//		        rightFlipperRotation -= 8;
	//	        },
	//	        true);

	Input::MouseInput::GetInstance()
	    .createKeymap(KEYMAPS::FREE_CAMERA)
	    .addClickCallback(
	        KEYMAPS::FREE_CAMERA,
	        GLFW_MOUSE_BUTTON_LEFT,
	        []() -> void
	        {
		        std::cout << "Click izquierdo presionado\n";
	        })
	    .addClickCallback(
	        KEYMAPS::FREE_CAMERA,
	        GLFW_MOUSE_BUTTON_RIGHT,
	        []() -> void
	        {
		        std::cout << "Click derecho presionado\n";
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
	boneShader->loadShader("shaders/shader_bone.vert", "shaders/shader_bone.frag");
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
	    .addModel(MODELS::MAQUINA_PINBALL, "assets/Models/MaquinaPinball.obj")
	    .addModel(MODELS::FLIPPER, "assets/Models/Flipper.obj")
	    .addModel(MODELS::CANICA, "assets/Models/canica.obj")
	    .addModel(MODELS::MAQUINA_CRISTAL, Utils::PathUtils::getModelsPath().append("/MaquinaCristal.obj"))
	    .addModel(MODELS::JK_1, Utils::PathUtils::getModelsPath().append("/Marx/Base.obj"))
	    .addModel(MODELS::JK_2, Utils::PathUtils::getModelsPath().append("/Marx/Body.obj"))
	    .addModel(MODELS::JK_3, Utils::PathUtils::getModelsPath().append("/Marx/Brazo1.obj"))
	    .addModel(MODELS::JK_4, Utils::PathUtils::getModelsPath().append("/Marx/Brazo2.obj"))
	    .addModel(MODELS::JK_5, Utils::PathUtils::getModelsPath().append("/Marx/Brazo3.obj"))
	    .addModel(MODELS::JK_6, Utils::PathUtils::getModelsPath().append("/Marx/Rotor.obj"))
#ifndef DEBUG // Este modelo causa muchos problemas
	    .addModel(MODELS::DESTROYED_BUILDING, Utils::PathUtils::getModelsPath().append("/ExtraModels/Building.obj"))
#endif
	    .loadModels();
}

void InitLights()
{
	Lights::LightCollectionBuilder<Lights::DirectionalLight> directionalLightsBuilder(2);
	directionalLights = directionalLightsBuilder
	                        .addLight(Lights::DirectionalLight(1.0f, 1.0f, 1.0f,
	                                                           0.8f, 0.3f,
	                                                           0.0f, -1.0f, 0.0f))
	                        .addLight(Lights::DirectionalLight(1.0f, 1.0f, 1.0f,
	                                                           0.3f, 0.3f,
	                                                           0.0f, -1.0f, 0.0f))
	                        .build();

	Lights::LightCollectionBuilder<Lights::PointLight> pointLightsBuilder(3);
	pointLights = pointLightsBuilder
//	                  .addLight(Lights::PointLight(1.0f, 0.0f, 1.0f,
//	                                               0.8f, 0.3f,
//	                                               0.0f, 0.0f, 0.0f,
//	                                               1.0f, 0.01f, 0.001f))
	                  .build();
	Lights::LightCollectionBuilder<Lights::SpotLight> spotLightBuilder(3);
	spotLights = spotLightBuilder
//	                 .addLight(Lights::SpotLight(1.0f, 1.0f, 1.0f,
//	                                             0.8f, 0.3f,
//	                                             5.22617, 234.113, 1.82546,
//	                                             0.0f, -1.0f, 0.0f,
//	                                             1.0f, 0.0f, 0.01f,
//	                                             40.0f))
	                 .build();
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

	Material_brillante = Model::Material(4.0f, 256);
	Material_opaco = Model::Material(0.3f, 4);

	// Constantes para uniforms
	GLuint uProjection, uModel, uView, uEyePosition, uSpecularIntensity, uShininess, uTexOffset, uColor;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / (GLfloat) mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	Utils::ModelMatrix handler(glm::mat4(1.0f));
	glm::mat4 model(1.0f);
	glm::mat4 modelaux(1.0f);
	glm::vec3 color(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	// modelos
	auto maquinaPinball = models[MODELS::MAQUINA_PINBALL];
	auto cristal = models[MODELS::MAQUINA_CRISTAL];
	auto flipper = models[MODELS::FLIPPER];
	auto mj1 = models[MODELS::JK_1];
	auto mj2 = models[MODELS::JK_2];
	auto mj3 = models[MODELS::JK_3];
	auto mj4 = models[MODELS::JK_4];
	auto mj5 = models[MODELS::JK_5];
	auto mj6 = models[MODELS::JK_6];
#ifndef DEBUG
	auto destroyedBuilding = models[MODELS::DESTROYED_BUILDING];
#endif

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

		// region Modelo Jerarquico 1
		// Para tomar las coordenadas de Blender
		// y <-> z
		// z -> -z
		model = handler
		            .setMatrix(glm::mat4(1.0f))
		            .translate(15.5f, 58.287f, -9.987f)
		            .rotateZ(6)
		            .rotateY((float) (15 * glfwGetTime()))
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj1.render();

		model = handler.setMatrix(modelaux)
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj2.render();

		model = handler.setMatrix(modelaux)
		            .translate(0.0f, 2.9f, -3.2f)
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj3.render();

		model = handler.setMatrix(modelaux)
		            .translate(0.0f, 4.13f, -0.19f)
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj4.render();

		model = handler.setMatrix(modelaux)
		            .translate(0.0f, 0.41f, -3.3f)
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj5.render();

		model = handler.setMatrix(modelaux)
		            .translate(0.0f, 0.186f, -2.01f)
		            .rotateX((float) (70 * glfwGetTime()))
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj6.render();
		// endregion Modelo Jerarquico 1

		// region Modelo Jerarquico 2
		model = handler
		            .setMatrix(glm::mat4(1.0f))
		            .translate(6.336f, 57.283f, 5.82f)
		            .rotateZ(6)
		            .rotateY((float) (15 * glfwGetTime()))
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj1.render();

		model = handler.setMatrix(modelaux)
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj2.render();

		model = handler.setMatrix(modelaux)
		            .translate(0.0f, 2.9f, -3.2f)
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj3.render();

		model = handler.setMatrix(modelaux)
		            .translate(0.0f, 4.13f, -0.19f)
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj4.render();

		model = handler.setMatrix(modelaux)
		            .translate(0.0f, 0.41f, -3.3f)
		            .saveActualState(modelaux)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj5.render();

		model = handler.setMatrix(modelaux)
		            .translate(0.0f, 0.186f, -2.01f)
		            .rotateX((float) (70 * glfwGetTime()))
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		mj6.render();
		// endregion Modelo Jerarquico 2

		// region Extra models
		// region Destroyed Buildings
#ifndef DEBUG
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
#endif
		// endregion
		// endregion
		
		// region ALPHA

		// region Cristal
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0f);
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		cristal.render();
		glDisable(GL_BLEND);
		// endregion

		// endregion

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	Audio::AudioDevice::Terminate();
	exitProgram();

	return 0;
}
#pragma clang diagnostic pop