#include "Window.h"
#include <iostream>

Window mainWindow;

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

	while (!mainWindow.shouldClose())
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mainWindow.swapBuffers();
	}

	return 0;
}