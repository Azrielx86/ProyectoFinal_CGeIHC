#include "MouseInput.h"
namespace Input
{
MouseInput *MouseInput::instance = nullptr;

MouseInput &MouseInput::GetInstance()
{
	if (instance == nullptr)
		instance = new MouseInput();
	return *instance;
}
void MouseInput::toggleMouseEnabled()
{
	lastxPos = 0;
	lastyPos = 0;
	mouseEnabled = !mouseEnabled;
}

bool MouseInput::isMouseEnabled() const { return mouseEnabled; }

void MouseInput::handleClick(int button, int action, [[maybe_unused]] int mode) // NOLINT(*-convert-member-functions-to-static)
{
	if (currentKeymap == nullptr) return;
	if (button == GLFW_KEY_UNKNOWN) return;

	auto btn = &currentKeymap->at(button);
	//	currentKeymap->at(button).action = action;
	if (((btn->action & 0x3) != 0) && action == GLFW_RELEASE)
		if (btn->releaseCallback != nullptr)
			btn->releaseCallback();

	btn->action = action;
	btn->pressed = action != GLFW_RELEASE;
	for (const auto &k : *currentKeymap)
	{
//		if (k.action == GLFW_PRESS || (k.action == GLFW_REPEAT && k.repeat))
		if (k.action == GLFW_PRESS || (k.pressed && k.repeat))
			if (k.callback != nullptr)
				k.callback();
	}
}

void MouseInput::handlePosition(float xPos, float yPos)
{
	if (currentMoveFunction == nullptr) return;
	if (!mouseEnabled) return;
	if (firstMove)
	{
		lastxPos = xPos;
		lastyPos = yPos;
		firstMove = false;
	}

	xChange = xPos - lastxPos;
	yChange = lastyPos - yPos;

	lastxPos = xPos;
	lastyPos = yPos;
	(*currentMoveFunction)(0);
}

float MouseInput::getXChange() const
{
	return xChange;
}

float MouseInput::getYChange() const
{
	return yChange;
}

MouseInput &MouseInput::createKeymap(int keymap)
{
	auto clicks = std::vector<Click>(8);
	for (auto &key : clicks)
	{
		key.action = GLFW_RELEASE;
		key.repeat = false;
		key.callback = nullptr;
	}

	this->keymaps.insert(std::make_pair(keymap, clicks));
	this->moveMappings.insert(std::make_pair(keymap, nullptr));

	if (currentKeymap == nullptr)
		currentKeymap = &keymaps[keymap];

	if (currentMoveFunction == nullptr)
		currentMoveFunction = &moveMappings[keymap];
	return *this;
}
MouseInput &MouseInput::addClickCallback(int keymap, int key, const std::function<void()> &callback, bool repeat, const std::function<void()> &releaseCallback)
{
	keymaps[keymap].at(key).callback = callback;
	keymaps[keymap].at(key).repeat = repeat;
	keymaps[keymap].at(key).releaseCallback = releaseCallback;
	return *this;
}
MouseInput &MouseInput::addMoveCallback(int keymap, const std::function<void(float)> &callback)
{
	moveMappings[keymap] = callback;
	return *this;
}
} // namespace Input
