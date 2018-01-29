#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>



class OBJObject
{
private:

	/*
	//variables
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4 toWorld, m_translate, m_rotate, m_scale, m_orbit;
	float angle, point_size;
*/

public:
	OBJObject(const char* filepath, glm::vec3 in_ambient, glm::vec3 in_diffuse, glm::vec3 in_specular,	float in_shininess);
	~OBJObject();

	// shader variables for openGL
	GLuint VBO, NBO, VAO, EBO;
	GLuint uProjection, uModelview;

	// material properties
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	// methods
	void draw(GLuint);// shaderProgram);
	void check_max_min(glm::vec3 & point_to_check, float & out_x_max, float & out_x_min, float & out_y_max, float & out_y_min, float & out_z_max, float & out_z_min);

	void parse(const char* filepath);
	void draw();
	void rasterize_object(glm::mat4 m_DPV);

	void update();
	void spin(float);
	void translation(glm::vec3 v_translate);
	void scale(float scale_factor);
	void rotation(float rotation_angle_deg, glm::vec3 rotation_axis);
	void orbit(float rotation_angle_deg, glm::vec3 rotation_axis);
	void reset_position();
	void reset_orientation_scale();
	void change_point_size(float point_size_change);
	glm::mat4 get_world_mat();
	std::vector<glm::vec3> get_vertices_vec();
	std::vector<glm::vec3> get_normals_vec();

	std::vector<unsigned int> v_indices_norms;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4 toWorld, m_transformation, m_translate, m_rotate, m_scale, m_orbit, m_center_obj, m_center_obj_inv, m_default_scale;
	float angle, point_size, scale_factor;
	glm::vec3 v_translation;



};

#endif