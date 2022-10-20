#include "Mouse.h"

double mouse::x_ = 0;
double mouse::y_ = 0;

double mouse::last_x_ = 0;
double mouse::last_y_ = 0;

double mouse::dx_ = 0;
double mouse::dy_ = 0;

double mouse::scroll_dx_ = 0;
double mouse::scroll_dy_ = 0;

bool mouse::first_mouse_ = true;

bool mouse::buttons_[GLFW_MOUSE_BUTTON_LAST] = { false };
bool mouse::buttons_changed_[GLFW_MOUSE_BUTTON_LAST] = { false };

void mouse::cursor_pos_callback(GLFWwindow* window, const double x, const double y) {
	x_ = x;
	y_ = y;

	if (first_mouse_) {
		last_x_ = x_;
		last_y_ = y_;
		first_mouse_ = true;
	}

	dx_ = x - last_x_;
	dy_ = last_y_ - y; // inverted

	last_x_ = x_;
	last_y_ = y_;
}


void mouse::mouse_button_callback(GLFWwindow* window, const int button, const int action, int mode) {
	if (action != GLFW_RELEASE) {
		if (!buttons_[button]) {
			buttons_[button] = true;
		}
	}
	else
		buttons_[button] = false;

	buttons_changed_[button] = action != GLFW_REPEAT;
}

void mouse::mouse_wheel_callback(GLFWwindow* window, const double dx, const double dy) {
	scroll_dx_ = dx;
	scroll_dy_ = dy;
}

double mouse::get_mouse_x() {
	return x_;
}

double mouse::get_mouse_y() {
	return y_;
}

double mouse::get_dx() {
	const double ret = dx_;
	dx_ = 0;
	return ret;
}

double mouse::get_dy() {
	const double ret = dy_;
	dy_ = 0;
	return ret;
}

double mouse::get_scroll_dx() {
	const double ret = scroll_dx_;
	scroll_dx_ = 0;
	return ret;
}

double mouse::get_scroll_dy() {
	const double ret = scroll_dy_;
	scroll_dy_ = 0;
	return ret;
}

bool mouse::button(const int button) {
	return buttons_[button];
}

bool mouse::button_changed(const int button) {
	const double ret = buttons_changed_[button];
	buttons_changed_[button] = false;
	return ret;
}

bool mouse::button_went_up(const int button) {
	return !buttons_[button] && button_changed(button);
}

bool mouse::button_went_down(const int button) {
	return buttons_[button] && button_changed(button);
}
