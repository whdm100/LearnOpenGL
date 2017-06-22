#version 430 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D screenTexture;

void main()
{
	//color = vec4(vec3(1.0 - texture(screenTexture, texCoord)), 1.0);

	// set color gray
	color = texture(screenTexture, texCoord);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average, average, average, 1.0);
}