//
// Created by edgar on 10/1/2023.
//

#include "Window.h"

Window::Window() // NOLINT(*-pro-type-member-init)
{
	width = 1280;
	height = 720;
	title = "";
	//	for (bool &key : this->keys)
	//		key = false;
}

Window::Window(GLuint windowWidth, GLuint windowHeight, const char *windowTitle) // NOLINT(*-pro-type-member-init)
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

	bindCallbacks();

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);
	glfwSetWindowUserPointer(window, this);

	projection = glm::perspective(45.0f, (GLfloat) bufferWidth / (GLfloat) bufferHeight, 0.1f, 1000.0f);

	// Cuando ya se tengan elementos 3D habilitar esto
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	glfwSetCursorPosCallback(window, Window::handleMousePos);
	glfwSetMouseButtonCallback(window, Window::handleMouseClick);
	glfwSetScrollCallback(window, Window::handleMouseScroll);
	glfwSetWindowSizeCallback(window, Window::windowResizeCallback);
}

void Window::handleKeyboard([[maybe_unused]] GLFWwindow *window, int key, int code, int action, int mode)
{
	Input::KeyboardInput::GetInstance().handleKey(key, code, action, mode);
}

void Window::handleMousePos([[maybe_unused]] GLFWwindow *window, double xPos, double yPos)
{
	Input::MouseInput::GetInstance().handlePosition((float) xPos, (float) yPos);
}

void Window::handleMouseClick([[maybe_unused]] GLFWwindow *window, int button, int action, int mode)
{
	Input::MouseInput::GetInstance().handleClick(button, action, mode);
}

void Window::handleMouseScroll([[maybe_unused]] GLFWwindow *window, double xoffset, double yoffset)
{
	Input::MouseInput::GetInstance().handleScroll(xoffset, yoffset);
}

void Window::toggleMouse()
{
	if (Input::MouseInput::GetInstance().isMouseEnabled())
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::windowResizeCallback(GLFWwindow *window, int width, int height)
{
	auto pWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
	glViewport(0, 0, width, height);
	glfwGetFramebufferSize(window, &pWindow->bufferWidth, &pWindow->bufferHeight);
	pWindow->projection = glm::perspective(45.0f, (GLfloat) pWindow->bufferWidth / (GLfloat) pWindow->bufferHeight, 0.1f, 1000.0f);
}

glm::mat4 Window::getProjectionMatrix()
{
	return projection;
}
