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
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_T,
	        []() -> void
	        {
		        std::cout << "Mouse disabled!";
		        Input::MouseInput::GetInstance().toggleMouseEnabled();
		        mainWindow.toggleMouse();
	        })
	    .addCallback(
	        KEYMAPS::FREE_CAMERA, GLFW_KEY_L,
	        []() -> void
	        {
		        if (ambLight == AMB_LIGHTS::DAY)
			        ambLight = AMB_LIGHTS::NIGHT;
		        else
			        ambLight = AMB_LIGHTS::DAY;
	        });

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
	cameras.addCamera(Camera::Camera(glm::vec3(0.0f, 60.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f));
	activeCamera = cameras.getAcviveCamera();
}

void InitModels()
{
	models
	    .addModel(MODELS::MAQUINA_PINBALL, "assets/Models/MaquinaPinball.obj")
	    .addModel(MODELS::FLIPPER, "assets/Models/Flipper.obj")
	    .addModel(MODELS::CANICA, "assets/Models/canica.obj")
	    .addModel(MODELS::MAQUINA_CRISTAL, Utils::PathUtils::getModelsPath().append("/MaquinaCristal.obj"));
	models.loadModels();
}

void InitLights()
{
	Lights::LightCollectionBuilder<Lights::DirectionalLight> directionalLightsBuilder(2);
	directionalLights = directionalLightsBuilder
	                        .addLight(Lights::DirectionalLight(1.0f, 1.0f, 1.0f,
	                                                           0.8f, 0.3f,
	                                                           0.0f, 0.0f, -1.0f))
	                        .addLight(Lights::DirectionalLight(1.0f, 1.0f, 1.0f,
	                                                           0.3f, 0.3f,
	                                                           0.0f, 0.0f, -1.0f))
	                        .build();
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
	glm::vec3 color(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	// modelos
	auto maquinaPinball = models[MODELS::MAQUINA_PINBALL];
	auto flipper = models[MODELS::FLIPPER];

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
		shaderLight->SetPointLights(nullptr, 0);
		Material_opaco.UseMaterial(uSpecularIntensity, uShininess);

		toffset = {0.0f, 0.0f};
		color = {1.0f, 1.0f, 1.0f};

		glUniform2fv((GLint) uTexOffset, 1, glm::value_ptr(toffset));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(0.0, -1.0, 0.0)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));
		maquinaPinball.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-58, 49, 10)
		            .rotateZ(6)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper.render();

		model = handler.setMatrix(glm::mat4(1.0f))
		            .translate(-58, 49, -19)
		            .rotateY(180)
		            .rotateZ(-6)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		flipper.render();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	Audio::AudioDevice::Terminate();

	return 0;
}
#pragma clang diagnostic pop