#version 430 core

out vec4 fColor;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // to NDC(normalized device coordnates)
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    fColor = vec4(vec3(depth), 1.0);
}