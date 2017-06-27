#version 430 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec4 vColor;

uniform vec2 offset[100];

void main()
{
	vec2 real_position = position + offset[gl_InstanceID];
	gl_Position = vec4(real_position, 0.0f, 1.0f);
	vColor = vec4(color, 1.0f);
}