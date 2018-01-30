#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec4 position_to_send;
in vec3 normal_to_send;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

// uniform variables
uniform int render_with_normals;
uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
//uniform vec4 emission; 
uniform float shininess; 


void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
//    color = vec4(1.0f, 0.41f, 0.7f, sampleExtraOutput);
	if (render_with_normals == 1)
	{
		color = vec4(normal_to_send, 1.f);
	}
	else
	{
//		color = vec4(1.0f, 0.41f, 0.7f, sampleExtraOutput);
		color = specular;
	}
}
