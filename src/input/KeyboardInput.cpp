//
// Created by edgar on 10/6/2023.
//

#include "KeyboardInput.h"
namespace Input
{
KeyboardInput *KeyboardInput::instance = nullptr;

KeyboardInput &KeyboardInput::GetInstance()
{
	if (instance == nullptr)
		instance = new KeyboardInput();
	return *instance;
}

KeyboardInput &KeyboardInput::createKeymap(int keymap)
{
	auto keys = std::vector<Key>(1024);
	for (auto &key : keys)
	{
		key.action = GLFW_RELEASE;
		key.repeat = false;
		key.callback = nullptr;
		key.pressed = false;
	}

	this->keymaps.insert(std::make_pair(keymap, keys));
	if (currentKeymap == nullptr)
		currentKeymap = &keymaps[keymap];
	return *this;
}

void KeyboardInput::handleKey(int key, [[maybe_unused]] int code, int action, [[maybe_unused]] int mode)
{
	if (key == GLFW_KEY_UNKNOWN)
		return;
#ifndef KB_VER2
	currentKeymap->at(key).action = action;
	currentKeymap->at(key).pressed = action != GLFW_RELEASE;
	for (auto &k : *currentKeymap)
	{
//		if (k.action == GLFW_PRESS || (k.action & 0x3 && k.repeat))
		if (k.action == GLFW_PRESS || (k.pressed && k.repeat))
			if (k.callback != nullptr)
				k.callback();
	}
#else
	auto k = &currentKeymap->at(key);
	k->action = action;
	k->pressed = action != GLFW_RELEASE;
	if (k->pressed)
		if (k->callback != nullptr)
			k->callback();
#endif
}

KeyboardInput &KeyboardInput::addCallback(int keymap, int key, const std::function<void()> &callback, bool repeat)
{
	keymaps[keymap].at(key).callback = callback;
	keymaps[keymap].at(key).repeat = repeat;
	return *this;
}

std::vector<KeyboardInput::Key> *KeyboardInput::getCurrentKeymap()
{
	// Para el control del mouse ha sido necesario agregar esto (Ya que requiere de deltaTime).
	return this->currentKeymap;
}
void KeyboardInput::setKeymap(int id)
{
	for (auto &k : *currentKeymap)
	{
		k.action = GLFW_RELEASE;
		k.pressed = false;
	}
	this->currentKeymap = &this->keymaps.at(id);
	std::cout << "[ " << typeid(KeyboardInput).name() << " ] Input keymap changed.\n";
}
} // namespace Input
