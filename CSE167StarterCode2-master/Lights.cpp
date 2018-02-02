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
}

light::~light()
{
}

void light::send_gl_lights(GLuint shaderProgram)
{
	// link variables and send directional light
	gl_d_dir = glGetUniformLocation(shaderProgram, "dir_light.direction");
	glUniform4fv(gl_d_dir, 1, glm::value_ptr(dir_light.direction));
	gl_d_col = glGetUniformLocation(shaderProgram, "dir_light.color");
	glUniform4fv(gl_d_col, 1, glm::value_ptr(dir_light.color));

	// link variables and send point light
	gl_p_pos = glGetUniformLocation(shaderProgram, "p_light.position");
	glUniform4fv(gl_p_pos, 1, glm::value_ptr(p_light.position));	
	gl_p_col = glGetUniformLocation(shaderProgram, "p_light.color");
	glUniform4fv(gl_p_col, 1, glm::value_ptr(p_light.color));

	// link variables and send spot light
	gl_sp_pos = glGetUniformLocation(shaderProgram, "sp_light.position");
	glUniform4fv(gl_sp_pos, 1, glm::value_ptr(sp_light.position));
	gl_sp_dir = glGetUniformLocation(shaderProgram, "sp_light.direction");
	glUniform4fv(gl_sp_dir, 1, glm::value_ptr(sp_light.direction));
	gl_sp_col = glGetUniformLocation(shaderProgram, "sp_light.color");
	glUniform4fv(gl_sp_col, 1, glm::value_ptr(sp_light.color));
	gl_cone_angle = glGetUniformLocation(shaderProgram, "sp_light.cut_off_angle");
	glUniform1f(gl_cone_angle, sp_light.cut_off_angle);
	gl_taper = glGetUniformLocation(shaderProgram, "sp_light.exponent");
	glUniform1f(gl_taper, sp_light.exponent);
}