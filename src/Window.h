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
	//para animacion
	GLboolean getStartAnimacionPico1(){return StartAnimacionPico1; }
	GLboolean setStartAnimacionPico1TRUE() { return StartAnimacionPico1 = true; }
	GLboolean setStartAnimacionPico1FALSE() { return StartAnimacionPico1 = false; }
	GLboolean getStartAnimacionPico2() { return StartAnimacionPico2; }
	GLboolean setStartAnimacionPico2TRUE() { return StartAnimacionPico2 = true; }
	GLboolean setStartAnimacionPico2FALSE() { return StartAnimacionPico2 = false; }
	GLboolean getStartAnimacionPico3() { return StartAnimacionPico3; }
	GLboolean setStartAnimacionPico3TRUE() { return StartAnimacionPico3 = true; }
	GLboolean setStartAnimacionPico3FALSE() { return StartAnimacionPico3 = false; }
	GLboolean getStartAnimacionCanica() { return StartAnimacionCanica; }
	GLboolean setStartAnimacionCanicaTRUE() { return StartAnimacionCanica = true; }
	GLboolean setStartAnimacionCanicaFALSE() { return StartAnimacionCanica = false; }
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
};

#endif // PROYECTOFINAL_CGEIHC_WINDOW_H
       
/*	   
*	//0. X=-78.000000, Y=45.500000, Z=7.500000
    //1. X=-78.000000, Y=45.500000, Z=36.531509

	//3. X=21.989304, Y=55.978912, Z=36.531509
*/