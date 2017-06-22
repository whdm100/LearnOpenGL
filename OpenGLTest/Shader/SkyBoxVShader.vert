#version 430 core

layout (location = 0) in vec3 position;

out vec3 texcoord;

layout (std140) uniform allmat
{
	mat4 model;
	mat4 view;
	mat4 proj;
	mat4 trans;
};

layout (std140) uniform material
{
	vec4 emission;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 power;
};

void main()
{
	vec4 pos = trans * vec4(position, 1.0);
	gl_Position = pos.xyww;
	texcoord = position;
}