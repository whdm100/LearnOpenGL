#version 430 core

layout(triangles) in;
layout(line_strip, max_vertices=6) out;

in VS_OUT {
    vec3 vNormal;
} gs_in[];

const float line_length = 0.2;

void GenerateLine(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].vNormal, 0.0) * line_length;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	for (int i = 0; i < 3; ++i)
		GenerateLine(i);
}