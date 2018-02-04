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
	
	d_light_to_world = glm::mat4(1.f);
	p_light_to_world = glm::mat4(1.f);
	sp_pos_to_world = glm::mat4(1.f);
	sp_dir_to_world = glm::mat4(1.f);

}

light::~light()
{
}

void light::send_gl_lights(GLuint shaderProgram)
{
	glm::vec4 temp_vec;
	glm::mat4 dir_modelview = Window::V * d_light_to_world;
	glm::mat4 p_modelview = Window::V * p_light_to_world;
	glm::mat4 sp__pos_modelview = Window::V * sp_pos_to_world;
	glm::mat4 sp_dir_modelview = Window::V * sp_dir_to_world;



	// link variables and send directional light
	gl_d_dir = glGetUniformLocation(shaderProgram, "dir_light.direction");
	temp_vec = dir_modelview * dir_light.direction;
	glUniform4fv(gl_d_dir, 1, glm::value_ptr(temp_vec));
	gl_d_col = glGetUniformLocation(shaderProgram, "dir_light.color");
	glUniform4fv(gl_d_col, 1, glm::value_ptr(dir_light.color));


	// link variables and send point light
	gl_p_pos = glGetUniformLocation(shaderProgram, "p_light.position");
	temp_vec = p_modelview * p_light.position;
	glUniform4fv(gl_p_pos, 1, glm::value_ptr(temp_vec));
	gl_p_col = glGetUniformLocation(shaderProgram, "p_light.color");
	glUniform4fv(gl_p_col, 1, glm::value_ptr(p_light.color));


	// link variables and send spot light
	gl_sp_pos = glGetUniformLocation(shaderProgram, "sp_light.position");
	temp_vec = sp__pos_modelview * sp_light.position;
	glUniform4fv(gl_sp_pos, 1, glm::value_ptr(temp_vec));
	gl_sp_dir = glGetUniformLocation(shaderProgram, "sp_light.direction");
	temp_vec = sp_dir_modelview * sp_light.direction;
	glUniform4fv(gl_sp_dir, 1, glm::value_ptr(temp_vec));

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

void light::change_edge_exponent(float scale)
{
	sp_light.exponent = std::max(sp_light.exponent * scale, 0.f);
}

void light::rotate_light(int &light_num, float rot_angle_deg, glm::vec3 rotation_axis)
{
	if (light_num == 1)
	{
		d_light_rotation = glm::rotate(glm::mat4(1.0f), rot_angle_deg / 180.0f * glm::pi<float>(), rotation_axis);
//		d_light_to_world = d_light_to_world * d_light_rotation;
		d_light_to_world = d_light_rotation * d_light_to_world; // orbit rotation
		printf("rotating dir light\n");
	}
	else if (light_num == 2)
	{
		p_light_rotation = glm::rotate(glm::mat4(1.f), rot_angle_deg / 180.0f * glm::pi<float>(), rotation_axis);
		p_light_to_world = p_light_rotation * p_light_to_world;
		printf("rotating point light\n)");
	}
	else if (light_num == 3)
	{
		sp_light_rotation = glm::rotate(glm::mat4(1.f), rot_angle_deg / 180.0f * glm::pi<float>(), rotation_axis);
		sp_pos_to_world = sp_light_rotation * sp_pos_to_world;
		sp_dir_to_world = sp_light_rotation * sp_dir_to_world;
	}
}

void light::scale_light(int &light_num, float mouse_offset)
{
	float scale_factor = 0;
	if (mouse_offset > 0.f) { scale_factor = 1.25f; }
	else if (mouse_offset < 0.f) { scale_factor = .75f; }
	glm::mat4 scale_matrix = glm::scale(glm::mat4(1.f), glm::vec3(scale_factor));

	if (light_num ==2)
	{
		p_light_to_world = scale_matrix * p_light_to_world;
	}
	if (light_num == 3)
	{
		sp_pos_to_world = scale_matrix * sp_pos_to_world;
	}
}