//
// Created by edgar on 10/6/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_INPUT_H
#define PROYECTOFINAL_CGEIHC_INPUT_H

#include <GLFW/glfw3.h>
#include <functional>
#include <vector>
class Input
{
	typedef void (*keyCallback)();

  public:
	Input(GLFWwindow *win);
	void AddCallback(int key, keyCallback callback);
	static void handleKeyboard(GLFWwindow *window, int key, int code, int action, int mode);
	Input *getPointer() { return this; }

  private:
	GLFWwindow *window;
	bool keys[1024];
	keyCallback keyCallbacks[1024];
};

#endif // PROYECTOFINAL_CGEIHC_INPUT_H
