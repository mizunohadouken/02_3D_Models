#version 330 core
// This is a sample fragment shader.

// light structs
struct directional_light
{
	vec4 direction;
	vec4 color;
};

struct point_light
{
	vec4 position;
	vec4 color;	
};

struct spot_light
{
	vec4 position;
	vec4 direction;
	vec4 color;
	float cut_off_angle;
	float exponent;
};

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec4 frag_pos;
in vec3 frag_normal;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

// uniform variables
uniform int render_with_normals;
uniform int light_mode;

// materials
uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
//uniform vec4 emission; 
uniform float shininess; 
// camera eye for light calculations
uniform vec3 eye_position;
// lights
uniform directional_light dir_light;
uniform point_light p_light;
uniform spot_light sp_light;

vec4 calc_dir_contribution(vec3 vertex_norm, vec3 to_eye)
{
		vec3 no_hom_light_dir = vec3(dir_light.direction.xyz);
		vec3 to_light = normalize(-no_hom_light_dir);
		
	    float nDotL = dot(vertex_norm, to_light)  ;         
        vec4 lambert = diffuse * dir_light.color * max (nDotL, 0.0);  
		
		vec3 relfective = reflect(-to_light, vertex_norm);
		float reflectiveDotEyeDir = dot(relfective, to_eye);
		vec4 phong = specular * dir_light.color * pow(max(reflectiveDotEyeDir, 0.0), shininess);
		
		return (lambert + phong); // TODO change
}

void main()
{
	vec4 dir_col_contrib = vec4(0.f);
	vec4 p_col_contrib = vec4(0.f);
	vec4 spot_col_contrib = vec4(0.f);

	// normalize current vertex normal and vertex position
	vec3 norm = normalize(frag_normal);
	vec3 dehom_frag_pos = frag_pos.xyz/frag_pos.w;
	vec3 to_eye = normalize(eye_position - dehom_frag_pos);

    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
//    color = vec4(1.0f, 0.41f, 0.7f, sampleExtraOutput);
	if (render_with_normals == 1)
	{
		color = vec4(frag_normal, 1.f);
//		color = vec4(eye_position/40.f,1.f);
	}
	else // use phong model
	{
//	    float nDotL = dot(normal, direction)  ;         
//        vec4 lambert = diffuse * lightcolor * max (nDotL, 0.0);  
		
//		color = vec4(.0f, 0.0f, 0.5f, 1.f);
//		color = sp_light.color;

// color = vec4(0.4f, 0.5f, 0.6f, 1.f);

		dir_col_contrib = calc_dir_contribution(norm, to_eye);
		color = ambient + dir_col_contrib + p_col_contrib + spot_col_contrib;
	}
}
