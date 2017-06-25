#version 430 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 coord;

out vec4 diffuse;
out vec4 specular;
out vec2 texcoord;

struct DirectionLight
{
	vec3 direction;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;	
};

uniform mat4 model;
uniform mat4 transform;
uniform DirectionLight dirLight;
uniform vec3 viewPos;

void main()
{
    gl_Position = transform * vec4(position, 1.0f);
	texcoord = coord;

	// diffuse color
	vec3 fLightDir = normalize(dirLight.direction);
	vec3 fNormal = mat3(transpose(inverse(model))) * normal;
	vec3 fLookDir = normalize(viewPos - position);

	diffuse = max(dot(fLightDir, fNormal), 0.0) * dirLight.diffuse + dirLight.ambient;
	
	// specular color
	vec3 fReflect = reflect(-fLightDir, fNormal);
	specular = dirLight.specular * max(dot(fLookDir, fReflect), 0.0);
}