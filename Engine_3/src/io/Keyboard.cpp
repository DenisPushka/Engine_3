#include "Keyboard.h"

bool keyboard::keys_[GLFW_KEY_LAST] = { false };
bool keyboard::keys_changed_[GLFW_KEY_LAST] = { false };

void keyboard::key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mods) {
	if (action != GLFW_RELEASE) {
		if (!keys_[key]) {
			keys_[key] = true;
		}
	}
	else
		keys_[key] = false;

	keys_changed_[key] = action != GLFW_REPEAT;
}

bool keyboard::key(const int key) {
	return keys_[key];
}

bool keyboard::key_changed(const int key) {
	const bool ret = keys_changed_[key];
	keys_changed_[key] = false;
	return ret;
}

bool keyboard::key_went_up(const int key) {
	return !keys_[key] && key_changed(key);
}

bool keyboard::key_went_down(const int key) {
	return keys_[key] && key_changed(key);
}
