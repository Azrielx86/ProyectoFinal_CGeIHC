#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GlobalConstants.h"
#include "Shader.h"
#include "Utils/ModelMatrix.h"
#include "Window.h"
#include "camera/Camera.h"
#include "camera/CameraCollection.h"
#include "input/KeyboardInput.h"
#include "model/BasicPrimitives.h"
#include "Audio/AudioDevice.h"

Window mainWindow;
BasicPrimitives primitives;
Camera::CameraCollection cameras;
Camera::Camera *activeCamera;

std::unordered_map<int, Shader *> shaders;

float deltaTime = 0.0f;
float lastTime = 0.0f;
const float limitFPS = 1.0f / 60.0f;

// Constantes para uniforms
GLuint uProjection = 0, uModel = 0, uView = 0, uEyePosition = 0,
       uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
GLuint uColor = 0;

void InitKeymaps()
{
	Input::KeyboardInput::GetInstance()
	    .createKeymap(Keymaps::FREE_CAMERA)
	    .addCallback(
	        Keymaps::FREE_CAMERA, GLFW_KEY_ESCAPE,
	        []() -> void
	        {
		        glfwSetWindowShouldClose(mainWindow.getWindowPointer(), GL_TRUE);
	        })
	    .addCallback(
	        Keymaps::FREE_CAMERA, GLFW_KEY_T,
	        []() -> void
	        {
		        std::cout << "Mouse disabled!";
		        Input::MouseInput::GetInstance().toggleMouseEnabled();
		        mainWindow.toggleMouse();
	        });

	Input::MouseInput::GetInstance()
	    .createKeymap(Keymaps::FREE_CAMERA)
	    .addClickCallback(
	        Keymaps::FREE_CAMERA,
	        GLFW_MOUSE_BUTTON_LEFT,
	        []() -> void
	        {
		        std::cout << "Click izquierdo presionado\n\n";
	        })
	    .addMoveCallback(
	        Keymaps::FREE_CAMERA,
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
	cameras.addCamera(Camera::Camera(glm::vec3(0.0f, 2.0f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f));
	activeCamera = cameras.getAcviveCamera();
}

int main()
{
	mainWindow = Window(1280, 720, "Proyecto :P");

	if (!mainWindow.Init())
	{
		std::cerr << "No se pudo iniciar la ventana\n";
		return 1;
	}

	// Inicializar los componentes del programa
	InitShaders();
	InitKeymaps();
	InitCameras();
	
	Audio::AudioDevice::GetInstance();

	// Matriz para transformaciones
	Utils::ModelMatrix handler(glm::mat4(1.0f));

	// Prueba de primitivas
	primitives.CreatePrimitives();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / (GLfloat) mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	glm::mat4 model(1.0f);
	glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

	while (!mainWindow.shouldClose())
	{
		auto now = (float) glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		glfwPollEvents();
		activeCamera->keyControl(Input::KeyboardInput::GetInstance(), deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaders[Shader::ShaderTypes::BASE_SHADER]->useProgram();
		uModel = shaders[Shader::ShaderTypes::BASE_SHADER]->getUniformModel();
		uProjection = shaders[Shader::ShaderTypes::BASE_SHADER]->getUniformProjection();
		uView = shaders[Shader::ShaderTypes::BASE_SHADER]->getUniformView();
		uEyePosition = shaders[Shader::ShaderTypes::BASE_SHADER]->getUniformEyePosition();
		uColor = shaders[Shader::ShaderTypes::BASE_SHADER]->getUniformColor();

		glUniformMatrix4fv((GLint) uProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv((GLint) uView, 1, GL_FALSE, glm::value_ptr(activeCamera->calculateViewMatrix()));
		auto camPos = activeCamera->getCameraPosition();
		glUniform3f((GLint) uEyePosition, camPos.x, camPos.y, camPos.z);

		// Renderizar primer primitiva
		color = {1.0f, 1.0f, 1.0f, 1.0f};
		model = handler
		            .setMatrix(glm::mat4(1.0f))
		            .scale(2.0f)
		            .getMatrix();
		glUniformMatrix4fv((GLint) uModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv((GLint) uColor, 1, glm::value_ptr(color));
		primitives.getPrimitive(BasicPrimitives::Primitives::FLOOR)->RenderMesh();

		mainWindow.swapBuffers();
	}

	Audio::AudioDevice::Terminate();
	
	return 0;
}