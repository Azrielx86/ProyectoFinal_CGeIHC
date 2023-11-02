#include "BasicPrimitives.h"
#include "Camera.h"
#include "Input.h"
#include "Shader.h"
#include "Window.h"

#include <iostream>

Window mainWindow;
BasicPrimitives primitives;

std::unordered_map<int, Shader *> shaders;

enum Keymaps
{
	CAMERA_PINBALL,
	CAMERA_AVATAR
};

void InitKeymaps()
{
	Input *input = Input::GetInstance();
	input->createKeymap(CAMERA_PINBALL);

	input->addCallback(CAMERA_PINBALL,
	                   GLFW_KEY_ESCAPE,
	                   []() -> void
	                   {
		                   glfwSetWindowShouldClose(mainWindow.getWindowPointer(), GL_TRUE);
	                   })
	    .addCallback(
	        CAMERA_PINBALL, GLFW_KEY_9,
	        []() -> void
	        {
		        std::cout << "9 presionado!\n";
	        },
	        true)
	    .addCallback(
	        CAMERA_PINBALL, GLFW_KEY_0,
	        []() -> void
	        {
		        std::cout << "0 presionado!\n";
	        },
	        true);
}

void InitShaders()
{
	auto shader = new Shader();
	shader->loadShader("shaders/shader.vert", "shaders/shader.frag");
	shaders[Shader::ShaderTypes::MODEL_TEX_SHADER] = shader;
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
	//	camera = Camera(glm::vec3(0.0f, 2.0f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

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