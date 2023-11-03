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
#include "input/MouseInput.h"
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
	void bindCallbacks();
	void toggleMouse();
	GLFWwindow *getWindowPointer() { return window; }
	static void handleKeyboard([[maybe_unused]] GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMousePos(GLFWwindow *window, double xPos, double yPos);
	static void handleMouseClick(GLFWwindow* window, int button, int action, int mode);

  private:
	GLFWwindow *window;
	GLuint width;
	GLuint height;
	int bufferWidth;
	int bufferHeight;
	const char *title;
};

#endif // PROYECTOFINAL_CGEIHC_WINDOW_H
