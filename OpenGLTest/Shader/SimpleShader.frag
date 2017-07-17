#version 430 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D texSampler1;
uniform sampler2D texSampler2;

void main()
{
	color = texture(texSampler1, texCoord);
	//color = mix(texture(texSampler1, texCoord), texture(texSampler2, texCoord), 0.5);
}