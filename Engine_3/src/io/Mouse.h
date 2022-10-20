#ifndef MOUSE_H
#define MOUSE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class mouse
{
public:
	static void cursor_pos_callback(GLFWwindow* window, double x, double y);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
	static void mouse_wheel_callback(GLFWwindow* window, double dx, double dy);

	static double get_mouse_x();
	static double get_mouse_y();

	static double get_dx();
	static double get_dy();

	static double get_scroll_dx();
	static double get_scroll_dy();

	static bool button(int button);
	static bool button_changed(int button);
	static bool button_went_up(int button);
	static bool button_went_down(int button);

private:
	static double x_;
	static double y_;

	static double last_x_;
	static double last_y_;

	static double dx_;
	static double dy_;
	static double scroll_dx_;
	static double scroll_dy_;

	static bool first_mouse_;
	static bool buttons_[];
	static bool buttons_changed_[];
};
#endif
