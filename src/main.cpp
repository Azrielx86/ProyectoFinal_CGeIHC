#include "GlobalConstants.h"
#include "Shader.h"
#include "Window.h"
#include "camera/Camera.h"
#include "camera/CameraCollection.h"
#include "input/KeyboardInput.h"
#include "model/BasicPrimitives.h"
#include <iostream>

Window mainWindow;
BasicPrimitives primitives;
Camera::CameraCollection cameras;
Camera::Camera *activeCamera;

std::unordered_map<int, Shader *> shaders;

void InitKeymaps()
{
	Input::KeyboardInput::GetInstance()
	    .createKeymap(Keymaps::CAMERA_PINBALL)
	    .addCallback(
	        Keymaps::CAMERA_PINBALL, GLFW_KEY_ESCAPE,
	        []() -> void
	        {
		        glfwSetWindowShouldClose(mainWindow.getWindowPointer(), GL_TRUE);
	        })
	    .addCallback(
	        Keymaps::CAMERA_PINBALL, GLFW_KEY_9,
	        []() -> void
	        {
		        std::cout << "9 presionado!\n";
	        },
	        true)
	    .addCallback(
	        Keymaps::CAMERA_PINBALL, GLFW_KEY_0,
	        []() -> void
	        {
		        std::cout << "0 presionado!\n";
	        },
	        true)
	    .addCallback(
	        Keymaps::CAMERA_PINBALL, GLFW_KEY_T,
	        []() -> void
	        {
		        std::cout << "Mouse disabled!";
		        Input::MouseInput::GetInstance().toggleMouseEnabled();
	        });

	Input::MouseInput::GetInstance()
	    .createKeymap(Keymaps::CAMERA_PINBALL)
	    .addClickCallback(
	        Keymaps::CAMERA_PINBALL,
	        GLFW_MOUSE_BUTTON_LEFT,
	        []() -> void
	        {
		        std::cout << "Click izquierdo presionado\n\n";
	        })
	    .addMoveCallback(
	        Keymaps::CAMERA_PINBALL,
	        [](float) -> void
	        {
		        activeCamera->mouseControl(Input::MouseInput::GetInstance());
	        });
}

void InitShaders()
{
	auto shader = new Shader();
	shader->loadShader("shaders/shader.vert", "shaders/shader.frag");
	shaders[Shader::ShaderTypes::MODEL_TEX_SHADER] = shader;
}

void InitCameras()
{
	cameras.addCamera(Camera::Camera(glm::vec3(0.0f, 2.0f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f));
	activeCamera = cameras.getAcviveCamera();
}

int main()
{
	mainWindow = Window(1280, 720, "Proyecto :P");

	//	mainWindow.createCallback(GLFW_KEY_W, cbakFromMain);
	//	mainWindow.createCallback(GLFW_KEY_R, scndCmakMain);

	if (!mainWindow.Init())
	{
		std::cerr << "No se pudo iniciar la ventana\n";
		return 1;
	}

	// Inicializar los componentes del programa
	InitShaders();
	InitKeymaps();
	InitCameras();

	// Prueba de primitivas
	primitives.CreatePrimitives();

	while (!mainWindow.shouldClose())
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders[Shader::ShaderTypes::MODEL_TEX_SHADER]->useProgram();

		primitives.getPrimitive(BasicPrimitives::Primitives::SINGLE_TRIANGLE)->RenderMesh();

		mainWindow.swapBuffers();
	}

	return 0;
}