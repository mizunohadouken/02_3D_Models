#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"

#include "OBJObject.h"

class Window
{
public:
	// variables
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view

	static double x_old, y_old;
	// send to switch between phong vs normal coloring
	static GLint phong_off;
	static GLuint normal_rendering;

	// send to determine lighting options
	static GLint light_option; // 0 for all lights, 1 for dir, 2 for point, 3 for spot
	static GLuint gl_light_option;

	// methods
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

	static glm::vec3 trackBallMapping(double & x_cursor, double & y_cursor);
};

#endif
