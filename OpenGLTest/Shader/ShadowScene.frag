#version 430 core

in VS_OUT
{
    vec2 texcoord;
} vs_out;

out vec4 color;

uniform sampler2D texSampler;

void main()
{
    color = texture(texSampler, vs_out.texcoord);
}