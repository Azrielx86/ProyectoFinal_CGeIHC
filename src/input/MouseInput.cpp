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
inline void MouseInput::toggleMouseEnabled() { mouseEnabled = !mouseEnabled; }
inline bool MouseInput::isMouseEnabled() const { return mouseEnabled; }
void MouseInput::handleClick(int button, int action, [[maybe_unused]] int mode) // NOLINT(*-convert-member-functions-to-static)
{
#ifdef DEBUG
	if (action == GLFW_PRESS)
		std::cout << "Pressed: ";
	else
		std::cout << "Released: ";
	const char *click = (button == GLFW_MOUSE_BUTTON_LEFT)    ? "Left button"
	                    : (button == GLFW_MOUSE_BUTTON_RIGHT) ? "Right button"
	                                                          : "Middle button";
	std::cout << click << "\n";
#endif
}

void MouseInput::handlePosition(float xPos, float yPos)
{
#ifdef DEBUG
	std::cout
	    << "[Mouse Position] (" << xPos
	    << ", " << yPos << ")\r";
#endif
	if (!mouseEnabled)
		return;

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
}
inline float MouseInput::getXChange() const
{
	return xChange;
}
inline float MouseInput::getYChange() const
{
	return yChange;
}
} // namespace Input
