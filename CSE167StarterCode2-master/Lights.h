#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

struct directional_light
{
	glm::vec4 direction;
	glm::vec4 color;
};

struct point_light
{
	glm::vec4 position;
	glm::vec4 color;
};

struct spot_light
{
	glm::vec4 position;
	glm::vec4 direction;
	glm::vec4 color;
	float cut_off_angle;
	float exponent;
};

class light
{
public:

	light(glm::vec4 d_dir, glm::vec4 d_col,
		glm::vec4 p_pos, glm::vec4 p_col,
		glm::vec4 sp_pos, glm::vec4 sp_dir, glm::vec4 sp_col, float cone_angle, float taper);
	~light();

	// methods
	void send_gl_lights(GLuint shaderProgram);

	// variables
	directional_light dir_light;
	point_light p_light;
	spot_light sp_light;
	// opengl varibles
	GLuint gl_d_dir, gl_d_col, gl_p_pos, gl_p_col, gl_sp_pos, gl_sp_dir, gl_sp_col, gl_cone_angle, gl_taper;


private:

};
