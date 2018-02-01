#define RENDERALL 1

#include "window.h"

#include <vector>
#include <memory>

int object_number = 0;
std::vector<std::unique_ptr<OBJObject>> v_objects_to_render;


const char* window_title = "GLFW Starter Project";
// TODO remove cube
//Cube * cube;
//OBJObject* obj_bunny;// = OBJObject("objs\\bunny.obj");

double Window::x_old;
double Window::y_old;
GLint Window::phong_off = 0;
GLuint Window::normal_rendering;

GLint Window::light_option = 0; // default to display all lights
GLuint Window::gl_light_option;
///////////////////
///////////////////

GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;


glm::mat4 Window::P;
glm::mat4 Window::V;


void Window::initialize_objects()
{
//	cube = new Cube();

	v_objects_to_render.push_back(std::unique_ptr<OBJObject> (new OBJObject("objs\\bunny.obj",
																			glm::vec4(0.25f, 0.25f, 0.25f, 1.f), // ambient
																			glm::vec4(0.f, 0.f, 0.f, 1.f), // diffuse
																			glm::vec4(.6f, .6f, .5f, 1.f), // specular
																			20.f)));					   // shininess
#if RENDERALL
	v_objects_to_render.push_back(std::unique_ptr<OBJObject>(new OBJObject("objs\\bear.obj",
																			glm::vec4(0.4f, 0.5f, 0.6f, 1.f), // ambient
																			glm::vec4(.7f, .7f, 1.f, 1.f), // diffuse
																			glm::vec4(0.f, 0.f, 0.f, 1.f), // specular
																			0.f)));						   // shininess
	v_objects_to_render.push_back(std::unique_ptr<OBJObject>(new OBJObject("objs\\dragon.obj",
																			glm::vec4(.5f, 0.4f, 0.3f, 1.f), // ambient
																			glm::vec4(.7f, .7f, 1.f, 1.f), // diffuse
																			glm::vec4(.6f, .6f, .5f, 1.f), // specular
																			20.f)));					   // shininess
#endif
//	obj_bunny = new OBJObject("objs\\bunny.obj");

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	// TODO remove cube
//	delete(cube);
	// TODO delete all objs
//	delete(obj_bunny);

	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
//	cube->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// TODO remove cube
	// Render the cube
//	cube->draw(shaderProgram);

	// TODO render objs
//	obj_bunny->draw(shaderProgram);
	v_objects_to_render[object_number]->draw(shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// press F1, F2, & F3 to display/render different OBJs
		else if (key == GLFW_KEY_F1) { object_number = 0; }
		else if (key == GLFW_KEY_F2) { object_number = 1; }
		else if (key == GLFW_KEY_F3) { object_number = 2; }
		
		// translate along x axis
		else if (key == GLFW_KEY_X)
		{
			if (mods == GLFW_MOD_SHIFT) { v_objects_to_render[object_number]->translation(glm::vec3(.5f, 0.f, 0.f)); }
			else { v_objects_to_render[object_number]->translation(glm::vec3(-.5f, 0.f, 0.f)); }
		}

		// translate along y axis
		if (key == GLFW_KEY_Y)
		{
			if (mods == GLFW_MOD_SHIFT) { v_objects_to_render[object_number]->translation(glm::vec3(0.f, 0.5f, 0.f)); }
			else { v_objects_to_render[object_number]->translation(glm::vec3(0.f, -.5f, 0.f)); }
		}

		// translate along z axis
		else if (key == GLFW_KEY_Z)
		{
			if (mods == GLFW_MOD_SHIFT) { v_objects_to_render[object_number]->translation(glm::vec3(0.f, 0.f, -1.f)); }
			else { v_objects_to_render[object_number]->translation(glm::vec3(0.f, 0.f, 1.f)); }
		}

		// scale object
		else if (key == GLFW_KEY_S)
		{
			if (mods == GLFW_MOD_SHIFT) { v_objects_to_render[object_number]->scale(1.5f); }
			else { v_objects_to_render[object_number]->scale(.5f); }
		}

		// orbit object about the z axis
		else if (key == GLFW_KEY_O)
		{
			if (mods == GLFW_MOD_SHIFT) { v_objects_to_render[object_number]->orbit(10.f, glm::vec3(0.f, 0.f, 1.f)); }
			else { v_objects_to_render[object_number]->orbit(10.f, glm::vec3(0.f, 0.f, -1.f)); }
		}

		// reset position or orientation/scale
		else if (key == GLFW_KEY_R)
		{
			// upper case R resets scale and orientation
			if (mods == GLFW_MOD_SHIFT) { v_objects_to_render[object_number]->reset_orientation_scale(); }
			// lower case r resets position
			else { v_objects_to_render[object_number]->reset_position(); }
		}
		// switch between rendering opengl with normals or phong
		else if (key == GLFW_KEY_N)
		{
			normal_rendering = glGetUniformLocation(shaderProgram, "render_with_normals");
			printf("phong: %d\n", phong_off);
			if (phong_off == 0)
			{
				phong_off = 1;
				glUniform1i(normal_rendering, phong_off);
			}
			else
			{
				phong_off = 0;
				glUniform1i(normal_rendering, phong_off);
			}
		}
		//use keys 0, 1, 2, and 3 to switch between light modes
		else if (key == GLFW_KEY_0)
		{
			light_option = 0;
			gl_light_option = glGetUniformLocation(shaderProgram, "light_mode");
			printf("Light mode: %d - All lights on\n", light_option);
			glUniform1i(gl_light_option, light_option);
		}
		else if (key == GLFW_KEY_1)
		{
			light_option = 1;
			gl_light_option = glGetUniformLocation(shaderProgram, "light_mode");
			printf("Light mode: %d - Directional on\n", light_option);
			glUniform1i(gl_light_option, light_option);
		}
		else if (key == GLFW_KEY_2)
		{
			light_option = 2;
			gl_light_option = glGetUniformLocation(shaderProgram, "light_mode");
			printf("Light mode: %d - Point on\n", light_option);
			glUniform1i(gl_light_option, light_option);
		}
		else if (key == GLFW_KEY_3)
		{
			light_option = 3;
			gl_light_option = glGetUniformLocation(shaderProgram, "light_mode");
			printf("Light mode: %d - Spot on\n", light_option);
			glUniform1i(gl_light_option, light_option);
		}

	}
}

///////////////////////////////
//  additions for mouse actions
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
//	double xpos, ypos;
/*	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);


	if (state == GLFW_PRESS)
	{
		printf("right click\n");

		glfwGetCursorPos(window, &xpos, &ypos);
		printf("x: %f y: %f\n", xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		printf("left click\n");
	}*/
}

void printvec3(glm::vec3 vPrintMe)
{
	printf("Vector x: %f\n", vPrintMe.x);
	printf("Vector y: %f\n", vPrintMe.y);
	printf("Vector z: %f\n", vPrintMe.z);

}

void Window::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	float sensitivity_factor = .05f;

	if ((xpos > 0) && (xpos < width) && (ypos > 0) && (ypos < height))
	{
		int right_click_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (right_click_state == GLFW_PRESS){v_objects_to_render[object_number]->translation(sensitivity_factor * glm::vec3(xpos - x_old, y_old - ypos, 0.f));}

		glm::vec3 current_vec_pos = Window::trackBallMapping(xpos, ypos);
		glm::vec3 prev_vec_pos = Window::trackBallMapping(x_old, y_old);
		glm::vec3 mouse_direction = current_vec_pos - prev_vec_pos;
		float velocity = glm::length(mouse_direction);

		float angle = asin(glm::dot(prev_vec_pos, current_vec_pos) / (glm::length(prev_vec_pos) * glm::length(current_vec_pos)));
		glm::vec3 rotation_axis = glm::cross(prev_vec_pos, current_vec_pos);

		int left_click_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (left_click_state == GLFW_PRESS)	{v_objects_to_render[object_number]->rotation(angle, rotation_axis);}

		x_old = xpos;
		y_old = ypos;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// translate along z axis when using mouse scroll
	v_objects_to_render[object_number]->translation(glm::vec3(0.f, 0.f, yoffset));
}

glm::vec3 Window::trackBallMapping(double &x_cursor, double &y_cursor)
{
	glm::vec3 v;
	float d;

	v.x = ((2.f * x_cursor) - width) / width;
	v.y = (height - (2.f * y_cursor)) / height;
	v.z = 0.f;
	d = glm::length(v);
	d = (d < 1.f) ? d : 1.f;
	v.z = sqrtf(1.001f - (d * d));
	v = glm::normalize(v);
	return v;
}


//////////////
//////////////
