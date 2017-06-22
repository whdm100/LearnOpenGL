#version 430 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D texSampler1;

void main()
{
	vec4 texColor = texture(texSampler1, texCoord);
	//if (texColor.a < 0.01)
		//discard;
	color = texColor;
}