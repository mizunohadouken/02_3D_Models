#include "OBJObject.h"
#include "Window.h"

void OBJObject::draw(GLuint shaderProgram)
{
	// calculate model-view matrix
	glm::mat4 modelview = Window::V * toWorld;

	// link uniform variables from shader
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	gl_ambient = glGetUniformLocation(shaderProgram, "ambient");
	gl_diffuse = glGetUniformLocation(shaderProgram, "diffuse");
	gl_specular = glGetUniformLocation(shaderProgram, "specular");
	gl_shininess = glGetUniformLocation(shaderProgram, "shininess");


	// sent projection and model view matrix to uniform variables in shader
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	// send materials to shader
	glUniform4fv(gl_ambient, 1, glm::value_ptr(ambient));
	glUniform4fv(gl_diffuse, 1, glm::value_ptr(diffuse));
	glUniform4fv(gl_specular, 1, glm::value_ptr(specular));
	glUniform1f(gl_shininess, shininess);

	// bind vertex array to begin drawing
	glBindVertexArray(VAO);
	// draw with triangles using indices to determin order
	glDrawElements(GL_TRIANGLES, v_indices_norms.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void OBJObject::check_max_min(glm::vec3 &point_to_check, float &out_x_max, float &out_x_min, float &out_y_max, float &out_y_min, float &out_z_max, float &out_z_min)
{
	if (point_to_check.x > out_x_max) { out_x_max = point_to_check.x; }
	if (point_to_check.x < out_x_min) { out_x_min = point_to_check.x; }
	if (point_to_check.y > out_y_max) { out_y_max = point_to_check.y; }
	if (point_to_check.y < out_y_min) { out_y_min = point_to_check.y; }
	if (point_to_check.z > out_z_max) { out_z_max = point_to_check.z; }
	if (point_to_check.z < out_z_min) { out_z_min = point_to_check.z; }
}

OBJObject::OBJObject(const char *filepath, glm::vec4 in_ambient, glm::vec4 in_diffuse, glm::vec4 in_specular, float in_shininess)
{
	toWorld = glm::mat4(1.0f);
	m_translate = glm::mat4(1.0f);
	m_rotate = glm::mat4(1.0f);
	m_scale = glm::mat4(1.0f);
	m_orbit = glm::mat4(1.0f);
	glm::vec3 v_translation = glm::vec3(0.f);
	point_size = 1.f;
	scale_factor = 1.f;
	parse(filepath);
	this->angle = 0.f;

	// set material properties
	ambient = in_ambient;
	diffuse = in_diffuse;
	specular = in_specular;
	shininess = in_shininess;

	// send to opengl


	// TODO remove
	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error: " << err << std::endl;
		const char* str = (const char*)gluErrorString(err);
//		std::cerr << "OpenGL error : " << err << ", " << str << std::endl;
	}
	//end error check!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// tell open gl to allocate space for arrays and buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind vertex array, then buffers associated
	glBindVertexArray(VAO);

	// bind vbo and send vertices to layout location 0
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// send vertex atributes to vertex shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	
	
	// bind vbo and send normals  to layout location 1
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
	// send normal atributes to vertex shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	

	// bind ebo and send indices data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*v_indices_norms.size(), &v_indices_norms[0], GL_STATIC_DRAW);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*v_indices_norms.size(), &v_indices_norms[0], GL_STATIC_DRAW);
	
	
	// unbind currently bound buffer so no accidental changes
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// unbind VAO so no changes are made
	glBindVertexArray(0);
}

OBJObject::~OBJObject()
{
	// TODO delete OpenGL buffers
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char *filepath) 
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	std::cout << "parsing: " << filepath << "\n";

	FILE* fp;
//	float x, y, z;
	float r, g, b;
	int char1, char2;
	char lookahead[256];
	unsigned int point1, point2, point3, point4, point5, point6;
	glm::vec3 temp_vec, temp_normal;
	float out_x_max = -INFINITY, out_x_min = INFINITY, out_y_max = -INFINITY, out_y_min = INFINITY, out_z_max = -INFINITY, out_z_min = INFINITY;


	// open file to read and error out if not available
	fp = fopen(filepath, "rb");
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }

	int comment_count = 0, vert_count = 0, norm_count = 0, face_count = 0;

	while ((char1 = fgetc(fp)) != EOF)
	{
		if (char1 == '#')
		{
			fgets(lookahead, 256, fp);
			comment_count++;
		}
		else
		{
			char2 = fgetc(fp);
			if ((char1 == 'v') && (char2 == ' '))
			{
				fscanf(fp, "%f %f %f %f %f %f ", &temp_vec.x, &temp_vec.y, &temp_vec.z, &r, &g, &b);
				vertices.push_back(temp_vec);
				check_max_min(temp_vec, out_x_max, out_x_min, out_y_max, out_y_min, out_z_max, out_z_min);
				vert_count++;
			}
			else if ((char1 == 'v') && (char2 == 'n'))
			{
				fscanf(fp, "%f %f %f", &temp_vec.x, &temp_vec.y, &temp_vec.z);
				normals.push_back(temp_vec);
				norm_count++;
			}
			else if ((char1 == 'f') && (char2 == ' '))
			{
				fscanf(fp, "%u//%u %u//%u %u//%u", &point1, &point2, &point3, &point4, &point5, &point6);
				v_indices_norms.push_back(point1-1);
			//	v_indices_norms.push_back(point2);
				v_indices_norms.push_back(point3 - 1);
			//	v_indices_norms.push_back(point4);
				v_indices_norms.push_back(point5 - 1);
			//	v_indices_norms.push_back(point6);
				face_count++;
			}
		}
	}


	//calculate default scale within the window
	float x_range = abs(out_x_max - out_x_min);
	float y_range = abs(out_y_max - out_y_min);
	float z_range = abs(out_z_max - out_z_min);

	float max_scale = std::max(x_range, (std::max(y_range, z_range)));
	float scale_to_one = (10.f / max_scale);
	this->m_default_scale = glm::scale(glm::mat4(1.f), glm::vec3(scale_to_one));
	toWorld = toWorld * m_default_scale;

	/////////////


	// modify object to world matrix to center object
	float x_center, y_center, z_center;
	x_center = (out_x_max + out_x_min) / 2.f;
	y_center = (out_y_max + out_y_min) / 2.f;
	z_center = (out_z_max + out_z_min) / 2.f;
	glm::vec3 translation_to_center_vec = -glm::vec3(x_center, y_center, z_center);

	glm::mat4 m_translation = glm::translate(glm::mat4(1.f), translation_to_center_vec);
	this->m_center_obj = m_translation * this->m_center_obj;
	toWorld = toWorld * m_center_obj;
	
	/////////////
	this->m_center_obj_inv = glm::inverse(m_center_obj);
	////////////
	
	printf("Vertices: %lu\nNormals: %lu\nIndices: %lu\n", vertices.size(), normals.size(), v_indices_norms.size());
	printf("# Vert lines: %i\n", vert_count);
	printf("# Norm lines: %i\n", norm_count);
	printf("# Face lines: %i\n", face_count);

	printf("# Comment lines: %i\n\n", comment_count);

	printf("Max x: %f\n", out_x_max);
	printf("Min x: %f\n", out_x_min);
	printf("Max y: %f\n", out_y_max);
	printf("Min y: %f\n", out_y_min);
	printf("Max z: %f\n", out_z_max);
	printf("Min z: %f\n\n", out_z_min);

	/*
	for (int i = 0; i < v_indices_norms.size() / 2; i++)
	{
		printf("Vertex index: %u w/ normal index: %u\n", v_indices_norms[2 * i], v_indices_norms[2 * i + 1]);
	}
	*/

	fclose(fp);
}

void OBJObject::draw() 
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < ((v_indices_norms.size()) / 2); ++i)
	{
		glColor3f(normals[v_indices_norms[i*2]].x, normals[v_indices_norms[i*2]].y, normals[v_indices_norms[i]].z);
		glVertex3f(vertices[v_indices_norms[i*2]-1].x, vertices[v_indices_norms[i*2]-1].y, vertices[v_indices_norms[i*2]-1].z);
	}

/*
	glBegin(GL_POINTS);
	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		glColor3f(normals[i].x, normals[i].y, normals[i].z);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	*/

	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void OBJObject::rasterize_object(glm::mat4 m_DPV)
{

}

void OBJObject::update()
{
	spin(1.f);


//	this->toWorld = m_orbit * m_translate * m_center_obj * m_rotate * m_scale ;

}

void OBJObject::spin(float deg)
{
//	this->m_rotate= m_rotate*glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.f,1.f,0.f));

	toWorld = toWorld * m_center_obj_inv;

	this->m_rotate = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f));
	toWorld = toWorld * m_rotate;

	toWorld = toWorld * m_center_obj;


//	this->toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.f,1.f,0.f));
}

void OBJObject::translation(glm::vec3 v_translate)
{
//	this->m_translate = m_translate * glm::translate(glm::mat4(1.f), v_translate);
	this->m_translate = glm::translate(glm::mat4(1.f), v_translate);
	this->toWorld = m_translate * this->toWorld;	
	v_translation += v_translate;
	//	this->toWorld = glm::translate(this->toWorld, v_translate);
}

void OBJObject::scale(float scale_factor)
{
//	this->m_scale = m_scale * glm::scale(glm::mat4(1.f), glm::vec3(scale_factor));

	toWorld = toWorld * m_center_obj_inv;


	this->m_scale = glm::scale(glm::mat4(1.f), glm::vec3(scale_factor));
//	toWorld = glm::scale(glm::mat4(1.f), glm::vec3(scale_factor)) * toWorld;  // scale at current space
	toWorld = toWorld * glm::scale(glm::mat4(1.f), glm::vec3(scale_factor)); // scale with respect to object space

	toWorld = toWorld * m_center_obj;
}


void OBJObject::rotation(float rotation_angle_deg, glm::vec3 rotation_axis)
{
	toWorld = toWorld * m_center_obj_inv;


	m_rotate = glm::rotate(glm::mat4(1.0f), rotation_angle_deg / 180.0f * glm::pi<float>(), rotation_axis);
	toWorld = toWorld * m_rotate; // local rotation
//	toWorld = m_rotate * toWorld; // orbit rotation
//	toWorld = glm::rotate(toWorld, rotation_angle_deg / 180.0f * glm::pi<float>(), rotation_axis);


	toWorld = toWorld * m_center_obj;

}

void OBJObject::orbit(float rotation_angle_deg, glm::vec3 rotation_axis)
{
//	this->m_orbit = m_orbit * glm::rotate(glm::mat4(1.0f), rotation_angle_deg / 180.0f * glm::pi<float>(), rotation_axis);

	toWorld = toWorld * m_center_obj_inv;

	this->m_orbit = glm::rotate(glm::mat4(1.0f), rotation_angle_deg / 180.0f * glm::pi<float>(), rotation_axis);
	toWorld = m_orbit * toWorld;

	toWorld = toWorld * m_center_obj;

//	toWorld = glm::rotate(toWorld, rotation_angle_deg / 180.0f * glm::pi<float>(), rotation_axis);
}

void OBJObject::reset_position()
{
	
	toWorld[3][0] = 0.f;
	toWorld[3][1] = 0.f;
	toWorld[3][2] = 0.f;
	

	toWorld =toWorld * m_center_obj;

//	m_translate = glm::mat4(1.f);

}

void OBJObject::reset_orientation_scale()
{
//	m_orbit = m_rotate = m_scale = glm::mat4(1.f);

/*	glm::vec3 stored_translation = glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]);
	toWorld = glm::mat4(1.f);
	translation(stored_translation);*/


	toWorld = toWorld * m_center_obj_inv;
	glm::vec3 stored_translation = glm::vec3(toWorld[3][0], toWorld[3][1], toWorld[3][2]);
	toWorld = glm::mat4(1.f);
	toWorld = toWorld * m_default_scale;
	translation(stored_translation);

	toWorld = toWorld * m_center_obj;
}

void OBJObject::change_point_size(float point_size_change)
{
	point_size = point_size * point_size_change;
	glPointSize(point_size);
}

glm::mat4 OBJObject::get_world_mat()
{
	return toWorld;
}

std::vector<glm::vec3> OBJObject::get_vertices_vec()
{
	return vertices;
}

std::vector<glm::vec3> OBJObject::get_normals_vec()
{
	return normals;
}
