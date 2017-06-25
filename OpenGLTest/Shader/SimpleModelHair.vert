#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoord;

out VS_OUT
{
	vec3 vNormal;
} vs_out;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position, 1.0f);
	vs_out.vNormal = normalize(mat3(transpose(inverse(transform))) * normal);
}