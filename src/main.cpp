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

Window mainWindow;
Camera::CameraCollection cameras;
Camera::Camera *activeCamera;
Model::ModelCollection models;
Lights::LightCollection<Lights::DirectionalLight> directionalLights;
Lights::LightCollection<Lights::PointLight> pointLights;
Lights::LightCollection<Lights::SpotLight> spotLights;
Skybox skybox;

Model::Material Material_brillante;
Model::Material Material_opaco;

std::unordered_map<int, Shader *> shaders;

float deltaTime = 0.0f;
float lastTime = 0.0f;
const float limitFPS = 1.0f / 60.0f;

AMB_LIGHTS ambLight = AMB_LIGHTS::DAY;

// Variables auxiliares que no se encapsularon en otras clases :v
float rightFlipperRotation = 0.0f;
float leftFlipperRotation = 0.0f;

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
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_C,
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
	        }, true)
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_L,
	        []() -> void
	        {
		        if (ambLight == AMB_LIGHTS::DAY)
			        ambLight = AMB_LIGHTS::NIGHT;
		        else
			        ambLight = AMB_LIGHTS::DAY;
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
	shaders[Shader::ShaderTypes::BASE_SHADER] = shader;

	auto lightShader = new Shader();
	lightShader->loadShader("shaders/shader_light.vert", "shaders/shader_light.frag");
	shaders[Shader::ShaderTypes::LIGHT_SHADER] = lightShader;
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
		.addModel(MODELS::NORA,"assets/Models/NoraF.obj")
	    .addModel(MODELS::MUNECO, "assets/Models/muneco_Hielo.obj")
	    .addModel(MODELS::PICOP, "assets/Models/Pico_P.obj")
	    .addModel(MODELS::PICOM, "assets/Models/Pico_M.obj")
	    .addModel(MODELS::PICOG, "assets/Models/Pico_G.obj")
	    .addModel(MODELS::MAQUINA_CRISTAL, Utils::PathUtils::getModelsPath().append("/MaquinaCristal.obj"))
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
	                                                           0.0f, 0.0f, -1.0f))
	                        .build();

	Lights::LightCollectionBuilder<Lights::PointLight> pointLightsBuilder(3);
	pointLights = pointLightsBuilder
	                  .addLight(Lights::PointLight(1.0f, 0.0f, 1.0f,
	                                               0.8f, 0.3f,
	                                               0.0f, 0.0f, 0.0f,
	                                               1.0f, 0.01f, 0.001f))
	                  .build();
	Lights::LightCollectionBuilder<Lights::SpotLight> spotLightBuilder(3);
	spotLights = spotLightBuilder
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
	InitShaders();
	InitKeymaps();
	InitCameras();
	InitModels();
	InitLights();

	std::vector<std::string> skyboxFaces;

	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_rt.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_lf.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_dn.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_up.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_bk.png");
	skyboxFaces.emplace_back("assets/Textures/Skybox/sp2_ft.png");

	skybox = Skybox(skyboxFaces);

	Audio::AudioDevice::GetInstance();

	Material_brillante = Model::Material(4.0f, 256);
	Material_opaco = Model::Material(0.3f, 4);

	// Constantes para uniforms
	GLuint uProjection, uModel, uView, uEyePosition, uSpecularIntensity, uShininess, uTexOffset, uColor;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / (GLfloat) mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	Utils::ModelMatrix handler(glm::mat4(1.0f));
	glm::mat4 model(1.0f);
	glm::mat4 aux(1.0f);
	glm::vec3 color(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	// modelos
	auto maquinaPinball = models[MODELS::MAQUINA_PINBALL];
	auto flipper = models[MODELS::FLIPPER];
	auto Nora = models[MODELS::NORA];
	auto Muneco = models[MODELS::MUNECO];
	auto PicoP = models[MODELS::PICOP];
	auto PicoM = models[MODELS::PICOM];
	auto PicoG = models[MODELS::PICOG];
	// Shaders
	auto shaderLight = shaders[Shader::ShaderTypes::LIGHT_SHADER];

	while (!mainWindow.shouldClose())
	{
		auto now = (float) glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		activeCamera->keyControl(Input::KeyboardInput::GetInstance(), deltaTime);

		updateFlippers();

		// Configuración del shader
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(activeCamera->calculateViewMatrix(), projection);
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
		glUniformMatrix4fv((GLint) uProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv((GLint) uView, 1, GL_FALSE, glm::value_ptr(activeCamera->calculateViewMatrix()));
		auto camPos = activeCamera->getCameraPosition();
		glUniform3f((GLint) uEyePosition, camPos.x, camPos.y, camPos.z);

		// Iluminacion
		shaderLight->SetDirectionalLight(&directionalLights[ambLight]);
		shaderLight->SetSpotLights(nullptr, 0);
		shaderLight->SetPointLights(pointLights.getLightArray(), pointLights.getCurrentCount());
		Material_opaco.UseMaterial(uSpecularIntensity, uShininess);

		toffset = {0.0f, 0.0f};
		color = {1.0f, 1.0f, 1.0f};

		glUniform2fv((GLint) uTexOffset, 1, glm::value_ptr(toffset));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));

		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::mat4 modelaux3(1.0);
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(0.0, -1.0, 0.0)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));
		maquinaPinball.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-58, 48, 10)
		            .rotateZ(6)
		            .rotateY(rightFlipperRotation)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper.render();
		
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-58, 48, -19)
		            .rotateZ(-6)
		            .rotateY(180 + leftFlipperRotation)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper.render();

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

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(8, 55, -26)
		            .scale(0.4)
		            .rotateZ(-6)
		            .rotateY(180)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		Muneco.render();
		
		//1er pico animado
		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(15.5, 57.45, -10)
		            .saveActualState(modelaux)// modelaux = model;   
		            .scale(2.2)
					.rotateZ(6)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoG.render();
		//1
		model = handler.setMatrix(modelaux)
		            .translate(-2.5, 1.55, 0)
		            .scale(2.2)
		            .rotateZ(44.39)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//2
		model = handler.setMatrix(modelaux)
		            .translate(.5, 3.55, +2)
					.rotateZ(-8)
		            .rotateX(48)
					.scale(2.2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//3
		model = handler.setMatrix(modelaux)
		            .translate(.5, 3.55, -2)
		            .rotateZ(-8)
		            .rotateX(-48)
					.scale(2.2)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//4
		model = handler.setMatrix(modelaux)
		            .translate(-0.7, 5.55, +.5)
		            .scale(2.2)
		            .rotateZ(40)
		            .rotateX(24)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		//5
		model = handler.setMatrix(modelaux)
		            .translate(-0.5, 5.55, -.5)
		            .scale(2.2)
		            .rotateZ(25)
		            .rotateX(-20)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();
		// 5
		model = handler.setMatrix(modelaux)
		            .translate(-0.5, 5.55, -.5)
		            .scale(2.2)
		            .rotateZ(25)
		            .rotateX(-20)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		PicoM.render();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	Audio::AudioDevice::Terminate();

	return 0;
}
#pragma clang diagnostic pop