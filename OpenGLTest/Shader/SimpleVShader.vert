#version 430 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 coord;

out vec2 texCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0f);
	texCoord = vec2(coord.s, 1.0 - coord.t);
}