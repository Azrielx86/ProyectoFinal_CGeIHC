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
		bool pressed;
		std::function<void()> callback;
		bool repeat;
		std::function<void()> releaseCallback;
	} Click;

	MouseInput(MouseInput &) = delete;
	void operator=(const MouseInput &) = delete;
	static MouseInput &GetInstance();

	void handlePosition(float xPos, float yPos);
	void handleClick(int button, int action, int mode);
	static void handleScroll(double xoffset, double yoffset);

	void toggleMouseEnabled();
	[[nodiscard]] bool isMouseEnabled() const;
	[[nodiscard]] float getXChange() const;
	[[nodiscard]] float getYChange() const;
	[[nodiscard]] static float getYScroll();

	/**
	 * @return Actual keymap.
	 */
	std::vector<Click> *getCurrentKeymap() { return this->currentKeymap; }

	/**
	 * Creates a new keymap
	 * @param keymap keymap code [Recomended manage it by an enum]
	 * @return Input current instance.
	 */
	MouseInput &createKeymap(int keymap);

	/**
	 * Creates a callback for a key on a keymap.
	 * @param keymap Keymap code.
	 * @param key GLFW Key.
	 * @param callback Callback function for the key.
	 * @param repeat Enable it if the function must or can be called multiple times on a single press.
	 * @return Input current instance.
	 */
	MouseInput &addClickCallback(int keymap, int key, const std::function<void()> &callback, bool repeat = false, const std::function<void()> &releaseCallback = nullptr);

	MouseInput &addMoveCallback(int keymap, const std::function<void(float)> &callback);

  private:
	bool mouseEnabled = true;
	bool firstMove = true;
	float lastxPos = 0;
	float lastyPos = 0;
	float xChange = 0;
	float yChange = 0;
	static float yScroll;

	/*
	 * Cada "Modo del mouse debe contener lo siguiente:
	 *
	 * Eventos para click izquierdo, derecho y central.
	 * Evento para actualizar X y Y
	 */

	std::unordered_map<int, std::vector<Click>> keymaps;
	std::function<void(float)> *currentMoveFunction = nullptr;
	std::unordered_map<int, std::function<void(float)>> moveMappings;
	std::vector<Click> *currentKeymap = nullptr;
};
} // namespace Input
#endif // PROYECTOFINAL_CGEIHC_MOUSEINPUT_H
