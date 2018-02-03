#include "Lights.h"
#include "Window.h"

light::light(glm::vec4 d_dir, glm::vec4 d_col,
	glm::vec4 p_pos, glm::vec4 p_col,
	glm::vec4 sp_pos, glm::vec4 sp_dir, glm::vec4 sp_col, float cone_angle, float taper)
{
	dir_light.direction = d_dir;
	dir_light.color = d_col;
	
	p_light.position = p_pos;
	p_light.color = p_col;

	sp_light.position = sp_pos;
	sp_light.direction = sp_dir;
	sp_light.color = sp_col;
	sp_light.cut_off_angle = cone_angle;
	sp_light.exponent = taper;
	
	light_to_world = glm::mat4(1.f);
}

light::~light()
{
}

void light::send_gl_lights(GLuint shaderProgram)
{
	glm::mat4 modelview = Window::V * light_to_world;
	glm::vec4 temp_vec;

	// link variables and send directional light
	gl_d_dir = glGetUniformLocation(shaderProgram, "dir_light.direction");
	temp_vec = modelview * dir_light.direction;
	glUniform4fv(gl_d_dir, 1, glm::value_ptr(temp_vec));
//	glUniform4fv(gl_d_dir, 1, glm::value_ptr(dir_light.direction));
	gl_d_col = glGetUniformLocation(shaderProgram, "dir_light.color");
	glUniform4fv(gl_d_col, 1, glm::value_ptr(dir_light.color));


	// link variables and send point light
	gl_p_pos = glGetUniformLocation(shaderProgram, "p_light.position");
	temp_vec = modelview * p_light.position;
	glUniform4fv(gl_p_pos, 1, glm::value_ptr(temp_vec));
//	glUniform4fv(gl_p_pos, 1, glm::value_ptr(p_light.position));	
	gl_p_col = glGetUniformLocation(shaderProgram, "p_light.color");
	glUniform4fv(gl_p_col, 1, glm::value_ptr(p_light.color));


	// link variables and send spot light
	gl_sp_pos = glGetUniformLocation(shaderProgram, "sp_light.position");
	temp_vec = modelview * sp_light.position;
	glUniform4fv(gl_sp_pos, 1, glm::value_ptr(temp_vec));
//	glUniform4fv(gl_sp_pos, 1, glm::value_ptr(sp_light.position));

	gl_sp_dir = glGetUniformLocation(shaderProgram, "sp_light.direction");
	temp_vec = modelview * sp_light.direction;
	glUniform4fv(gl_sp_dir, 1, glm::value_ptr(temp_vec));
//	glUniform4fv(gl_sp_dir, 1, glm::value_ptr(sp_light.direction));

	gl_sp_col = glGetUniformLocation(shaderProgram, "sp_light.color");
	glUniform4fv(gl_sp_col, 1, glm::value_ptr(sp_light.color));

	//float cos_cut_off_rads = cos(sp_light.cut_off_angle / 180.0f * glm::pi<float>());
	gl_cone_angle = glGetUniformLocation(shaderProgram, "sp_light.cut_off_angle");
	glUniform1f(gl_cone_angle, sp_light.cut_off_angle);
	gl_taper = glGetUniformLocation(shaderProgram, "sp_light.exponent");
	glUniform1f(gl_taper, sp_light.exponent);
}

void light::transform_cone_angle(float scale)
{
	sp_light.cut_off_angle = std::max(sp_light.cut_off_angle + scale, 0.f);
}