//
// Created by edgar on 10/1/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_WINDOW_H
#define PROYECTOFINAL_CGEIHC_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <iostream>

class Window
{
  public:
	Window();
	explicit Window(GLuint windowWidth, GLuint windowHeight, const char *windowTitle = "");
	virtual ~Window();
	[[nodiscard]] int getBufferWidth() const { return bufferWidth; }
	[[nodiscard]] int getBufferHeight() const { return bufferHeight; }
	bool Init();
	bool shouldClose() { return glfwWindowShouldClose(window); }
	void swapBuffers() { return glfwSwapBuffers(window); }
	static void handleKeyboard(GLFWwindow *window, int key, int code, int action, int mode);

	// Creación de los callbacks (Para no usar tantos if)
	// fuente: https://gamedev.stackexchange.com/questions/150157/how-to-improve-my-input-handling-in-glfw
	using Callback = std::function<void()>;
	void createCallback(int key, const std::function<void()> &callback);
	void bindCallbacks();

  private:
	GLFWwindow *window;
	GLuint width;
	GLuint height;
	int bufferWidth;
	int bufferHeight;
	const char *title;
//	bool keys[1024];
	std::unordered_map<int, std::function<void()>> callbacks;
};

#endif // PROYECTOFINAL_CGEIHC_WINDOW_H
