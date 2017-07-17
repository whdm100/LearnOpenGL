#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

out VS_OUT
{
    vec2 texcoord;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 trans;

void main()
{
    gl_Position = trans * vec4(position, 1.0);
    vs_out.texcoord = vec2(texcoord.s, 1.0 - texcoord.t);
}