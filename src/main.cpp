#include "BasicPrimitives.h"
#include "Shader.h"
#include "Window.h"
#include <iostream>

Window mainWindow;
BasicPrimitives primitives;

void cbakFromMain() { std::cout << "Llamo esto desde un callback :D\n"; }
void scndCmakMain() { std::cout << "Otro callback desde el main!\n"; }

std::unordered_map<std::string, Shader*> shaders;

void InitShaders()
{
	auto shader = new Shader();
	shader->loadShader("shaders/shader.vert", "shaders/shader.frag");
	shaders["BasicShader"] = shader;
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
	
	// Prueba de primitivas
	primitives.CreatePrimitives();
	
	while (!mainWindow.shouldClose())
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders["BasicShader"]->useProgram();
		
		primitives.getPrimitive(BasicPrimitives::Primitives::SINGLE_TRIANGLE)->RenderMesh();

		mainWindow.swapBuffers();
	}

	return 0;
}