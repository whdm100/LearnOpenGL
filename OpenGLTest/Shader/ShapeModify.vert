#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out VS_OUT
{
	vec4 vColor;
} vs_out;

void main()
{
	vs_out.vColor = vec4(color, 1.0);
	gl_Position = vec4(position, 1.0);
}