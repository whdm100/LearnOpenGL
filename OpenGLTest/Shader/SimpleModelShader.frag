#version 430 core

in vec4 diffuse;
in vec4 specular;
in vec2 texcoord;

out vec4 fColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	vec4 dColor = vec4(texture(texture_diffuse1, texcoord)) * diffuse;
	vec4 sColor = vec4(texture(texture_specular1, texcoord)) * specular;
	fColor = dColor + sColor;
	//fColor = dColor;
}