#include "stdafx.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

Shader::Shader()
    : _program(0)
{
}


Shader::~Shader()
{
    Unload();
}

GLRESULT Shader::Load()
{
    if (!_program)
        _program = glCreateProgram();
    return GLOK;
}

void Shader::Unload()
{
    auto clear = [](const ShaderInfo& info)
    {
        glDeleteShader(info._id);
    };
    for_each(_shaders.begin(), _shaders.end(), clear);
    _shaders.clear();

    glDeleteProgram(_program);
    _program = 0;
}

GLRESULT Shader::LoadShader(const char *path, ShaderType type)
{
    GLRESULT ret = GLFAIL;
    FILE *file = fopen(path, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_SET);
        fseek(file, 0, SEEK_END);
        int len = ftell(file);

        if (len)
        {
            char *buff = new char[len + 1];
            memset(buff, 0, len + 1);

            fseek(file, 0, SEEK_SET);            
            ret = fread(buff, 1, len, file);

            ret = LoadShader(buff, len, type);

            delete[] buff;
        }
        fclose(file);
    }
    return ret;
}

GLRESULT Shader::LoadShader(const char *data, int len, ShaderType type)
{
    GLuint shader;

    shader = glCreateShader(GLShaderType(type));
    glShaderSource(shader, 1, &data, &len);

    // Compile shader source code
    glCompileShader(shader);
    if (GL_FAILED(CheckShaderCompile(shader)))
    {
        glDeleteShader(shader);
        return GLFAIL;
    }

    Bind();

    // Linking shader with program
    glAttachShader(_program, shader);
    glLinkProgram(_program);
    if (GL_FAILED(CheckShaderLinking(_program)))
    {
        glDeleteShader(shader);
        return GLFAIL;
    }

    UnBind();
    
    _shaders.push_back(ShaderInfo(shader, type));

    return GLOK;
}