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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	glm::mat4 getProjectionMatrix();
	static void handleKeyboard([[maybe_unused]] GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMousePos(GLFWwindow *window, double xPos, double yPos);
	static void handleMouseClick(GLFWwindow* window, int button, int action, int mode);
	static void handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	static void windowResizeCallback(GLFWwindow* window, int width, int height);

  private:
	GLFWwindow *window;
	GLuint width;
	GLuint height;
	GLboolean StartAnimacionPico1=false;
	GLboolean StartAnimacionPico2 = false;
	GLboolean StartAnimacionPico3 = false;
	GLboolean StartAnimacionCanica = false;
	int bufferWidth;
	int bufferHeight;
	const char *title;
	glm::mat4 projection;
};

#endif // PROYECTOFINAL_CGEIHC_WINDOW_H
       
/*	   
*	//0. X=-78.000000, Y=45.500000, Z=7.500000
    //1. X=-78.000000, Y=45.500000, Z=36.531509

	//3. X=21.989304, Y=55.978912, Z=36.531509
*/