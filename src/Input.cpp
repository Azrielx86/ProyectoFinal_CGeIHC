//
// Created by edgar on 10/6/2023.
//

#include "Input.h"

Input *Input::instance = nullptr;

Input *Input::GetInstance()
{
	if (instance == nullptr)
		instance = new Input();
	return instance;
}

Input &Input::createKeymap(int keymap)
{
	auto keys = std::vector<Key>(1024);
	for (auto &key : keys)
	{
		key.action = GLFW_RELEASE;
		key.repeat = false;
		key.callback = nullptr;
	}

	this->keymaps.insert(std::make_pair(keymap, keys));
	if (currentKeymap == nullptr)
		currentKeymap = &keymaps[keymap];
	return *this;
}

void Input::handleKey(int key, [[maybe_unused]] int code, int action, [[maybe_unused]] int mode)
{
	if (key == GLFW_KEY_UNKNOWN)
		return;

	currentKeymap->at(key).action = action;

	for (const auto &k : *currentKeymap)
	{
		if (k.action == GLFW_PRESS || (k.action == GLFW_REPEAT && k.repeat))
			if (k.callback != nullptr)
				k.callback();
	}
}

Input &Input::addCallback(int keymap, int key, const std::function<void()> &callback, bool repeat)
{
	keymaps[keymap].at(key).callback = callback;
	keymaps[keymap].at(key).repeat = repeat;
	return *this;
}
