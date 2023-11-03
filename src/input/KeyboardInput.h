//
// Created by edgar on 10/6/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_KEYBOARDINPUT_H
#define PROYECTOFINAL_CGEIHC_KEYBOARDINPUT_H

#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <vector>

namespace Input
{
/**
 * Input class, manages a global input system by a singleton instance.
 * It can use different keymaps.
 * @author Azrielx86 (Edgar Chalico)
 */
class KeyboardInput
{
  protected:
	explicit KeyboardInput(){};
	/**
	 * Single istance of the input manager.
	 */
	static KeyboardInput *instance;

  public:
	/**
	 * Represents a Key with its callback.
	 */
	typedef struct _key
	{
		int action;
		std::function<void()> callback;
		bool repeat;
		bool pressed;
	} Key;

	// Necesary for the singleton.
	KeyboardInput(KeyboardInput &) = delete;
	void operator=(const KeyboardInput &) = delete;

	/**
	 * Returns the Input manager instance, if it doesn' exists, creates a new one.
	 * @return Input manager global instance.
	 */
	static KeyboardInput &GetInstance();

	/**
	 * Creates a new keymap
	 * @param keymap keymap code [Recomended manage it by an enum]
	 * @return Input current instance.
	 */
	KeyboardInput &createKeymap(int keymap);

	/**
	 * Creates a callback for a key on a keymap.
	 * @param keymap Keymap code.
	 * @param key GLFW Key.
	 * @param callback Callback function for the key.
	 * @param repeat Enable it if the function must or can be called multiple times on a single press.
	 * @return Input current instance.
	 */
	KeyboardInput &addCallback(int keymap, int key, const std::function<void()> &callback, bool repeat = false);

	/**
	 * Handles the key events for GLFW.
	 * @param key GLFW Key.
	 * @param code unused.
	 * @param action GLFW Action.
	 * @param mode unused.
	 */
	void handleKey(int key, [[maybe_unused]] int code, int action, [[maybe_unused]] int mode);
	
	std::vector<Key> *getCurrentKeymap();

  private:
	std::unordered_map<int, std::vector<Key>> keymaps;
	std::vector<Key> *currentKeymap = nullptr;
};
} // namespace Input
#endif // PROYECTOFINAL_CGEIHC_KEYBOARDINPUT_H
