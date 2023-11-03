//
// Created by edgar on 11/2/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_MOUSEINPUT_H
#define PROYECTOFINAL_CGEIHC_MOUSEINPUT_H

#include <GLFW/glfw3.h>
#include <functional>

#ifdef DEBUG
#include <iostream>
#endif
namespace Input
{
class MouseInput
{
  protected:
	explicit MouseInput(){};
	/**
	 * Single istance of the input manager.
	 */
	static MouseInput *instance;

  public:
	typedef struct _click
	{
		int action;
		std::function<void()> callback;
		bool repeat;
	} Click;

	MouseInput(MouseInput &) = delete;
	void operator=(const MouseInput &) = delete;
	static MouseInput &GetInstance();

	void handlePosition(float xPos, float yPos);
	void handleClick(int button, int action, int mode);

	void toggleMouseEnabled();
	[[nodiscard]] bool isMouseEnabled() const;
	[[nodiscard]] float getXChange() const;
	[[nodiscard]] float getYChange() const;

  private:
	bool mouseEnabled = true;
	bool firstMove = true;
	float mousexPos = 0;
	float mouseyPos = 0;
	float lastxPos = 0;
	float lastyPos = 0;
	float xChange = 0;
	float yChange = 0;
	std::unordered_map<int, std::vector<Click>> keymaps;
	std::vector<Click> *currentKeymap = nullptr;
};
} // namespace Input
#endif // PROYECTOFINAL_CGEIHC_MOUSEINPUT_H
