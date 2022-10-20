#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class keyboard
{
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//accessors

	static bool key(int key);
	static bool key_changed(int key);
	static bool key_went_up(int key);
	static bool key_went_down(int key);
private:
	static bool keys_[];
	static bool keys_changed_[];
};
#endif
