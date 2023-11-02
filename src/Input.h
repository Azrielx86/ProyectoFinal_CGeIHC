//
// Created by edgar on 10/6/2023.
//

#ifndef PROYECTOFINAL_CGEIHC_INPUT_H
#define PROYECTOFINAL_CGEIHC_INPUT_H

#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <vector>
class Input
{
  protected:
	explicit Input() {};
	static Input *instance;
  public:
	typedef struct _key
	{
		int action;
		std::function<void()> callback;
		bool repeat;
	} Key;

//	Input(Input &) = delete;
//	void operator=(const Input &) = delete;
	static Input *GetInstance();
	Input &createKeymap(int keymap);
	Input &addCallback(int keymap, int key, const std::function<void()> &callback, bool repeat = false);
	void handleKey(int key, int code, int action, int mode);

  private:
	std::unordered_map<int, std::vector<Key>> keymaps;
	std::vector<Key>* currentKeymap = nullptr;
	
};
#endif // PROYECTOFINAL_CGEIHC_INPUT_H
