//
// Created by edgar on 10/1/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_WINDOW_H
#define PROYECTOFINAL_CGEIHC_WINDOW_H

// Mantener antes que input
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// ========================
#include "input/KeyboardInput.h"
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

	static void handleKeyboard([[maybe_unused]] GLFWwindow *window, int key, int code, int action, int mode);
	//
	//	void createCallback(int key, const std::function<void()> &callback);
	void bindCallbacks();
	GLFWwindow *getWindowPointer() { return window; }

  private:
	GLFWwindow *window;
	GLuint width;
	GLuint height;
	int bufferWidth;
	int bufferHeight;
	const char *title;
	//	static Input input;

	//	bool keys[1024];
	//	std::unordered_map<int, std::function<void()>> callbacks;
};

#endif // PROYECTOFINAL_CGEIHC_WINDOW_H
