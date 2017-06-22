#version 430 core

in vec3 texcoord;

out vec4 color;

uniform samplerCube cubemap;

void main()
{
	color = texture(cubemap, texcoord);
}