#include "BasicPrimitives.h"
#include "Camera.h"
#include "Shader.h"
#include "Window.h"
#include <iostream>

Window mainWindow;
BasicPrimitives primitives;
Camera camera;

void cbakFromMain() { std::cout << "Llamo esto desde un callback :D\n"; }
void scndCmakMain() { std::cout << "Otro callback desde el main!\n"; }

std::unordered_map<int, Shader *> shaders;

void InitShaders()
{
	auto shader = new Shader();
	shader->loadShader("shaders/shader.vert", "shaders/shader.frag");
	shaders[Shader::ShaderTypes::MODEL_TEX_SHADER] = shader;
}

int main()
{
	mainWindow = Window(1280, 720, "Proyecto :P");
	mainWindow.createCallback(GLFW_KEY_W, cbakFromMain);
	mainWindow.createCallback(GLFW_KEY_R, scndCmakMain);

	if (!mainWindow.Init())
	{
		std::cerr << "No se pudo iniciar la ventana\n";
		return 1;
	}

	// Inicializar los componentes del programa
	InitShaders();
	camera = Camera(glm::vec3(0.0f, 2.0f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

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