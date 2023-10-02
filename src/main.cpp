#include "BasicPrimitives.h"
#include "Window.h"
#include <iostream>

Window mainWindow;
BasicPrimitives primitives;

void cbakFromMain() { std::cout << "Llamo esto desde un callback :D\n"; }

int main(int argc, char **argv)
{
	mainWindow = Window(1280, 720, "Proyecto :P");
	mainWindow.createCallback(GLFW_KEY_W, cbakFromMain);

	if (!mainWindow.Init())
	{
		std::cerr << "No se pudo iniciar la ventana\n";
		return 1;
	}

	// Prueba de primitivas
	primitives.CreatePrimitives();
	
	while (!mainWindow.shouldClose())
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		primitives.getPrimitive(BasicPrimitives::Primitives::SINGLE_TRIANGLE)->RenderMesh();
		
		mainWindow.swapBuffers();
	}

	return 0;
}