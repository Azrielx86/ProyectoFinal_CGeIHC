//
// Created by edgar on 10/1/2023.
//

#include "Window.h"

Window::Window()
{
	width = 1280;
	height = 720;
	title = "";
	//	for (bool &key : this->keys)
	//		key = false;
}

Window::Window(GLuint windowWidth, GLuint windowHeight, const char *windowTitle)
{
	width = windowWidth;
	height = windowHeight;
	title = windowTitle;
	//	for (bool &key : this->keys)
	//		key = false;
}
bool Window::Init()
{
	if (!glfwInit())
	{
		std::cerr << "No se pudo iniciar GLFW\n";
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow((int) width, (int) height, title, nullptr, nullptr);

	if (!window)
	{
		std::cerr << "No se pudo crear la ventana\n";
		glfwTerminate();
		return false;
	}

	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(window);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "No se pudo inicializar GLEW\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	std::function<void()> exit = [this]() -> void
	{
		glfwSetWindowShouldClose(this->window, GL_TRUE);
	};

	//	this->createCallback(GLFW_KEY_ESCAPE, exit);

	bindCallbacks();

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);
	glfwSetWindowUserPointer(window, this);

	// Cuando ya se tengan elementos 3D habilitar esto
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	return true;
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::bindCallbacks()
{
	glfwSetKeyCallback(window, Window::handleKeyboard);
}
void Window::handleKeyboard(GLFWwindow *window, int key, int code, int action, int mode)
{
	auto input = Input::GetInstance();
	input->handleKey(key, code, action, mode);
}
