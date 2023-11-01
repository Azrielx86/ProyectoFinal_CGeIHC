//
// Created by edgar on 10/6/2023.
//

#include "Input.h"
Input::Input()
{
	for (auto &key : keys)
		key = false;
}

void Input::AddCallback(int key, Input::keyCallback callback) { this->keyCallbacks[key] = callback; }

void Input::handleKeyboard(GLFWwindow *window, int key, int code, int action, int mode)
{
//	auto input = static_cast<Input *>(glfwGetWindowUserPointer(window));
//	if (action == GLFW_PRESS)
//		keys[key] = true;
//	else if (action == GLFW_RELEASE)
//		keys[key] = false;
}
