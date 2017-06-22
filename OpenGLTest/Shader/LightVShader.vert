#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

out vec2 texcoord1;
out vec3 fNormal;
out vec3 fPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0f);

	texcoord1 = texcoord;

	fPos = vec3(model * vec4(position,1.0f));

	fNormal = normalize(normal);

	// to view space
	//mat3 viewMat3 = mat3(view);
	//mat3 transNormal = transpose(inverse(viewMat3));
	//fNormal = normalize(transNormal * normal);
}