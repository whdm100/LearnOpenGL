/**
 * @file: Shader.h
 *
 * @desc: Base shaders define.
 *
 * @version: 1.0
 *
 * @date: 2017/05/28
 *
 * @auther:
 *
 */
#ifndef __SHADER_H
#define __SHADER_H

#include "GLImp.h"
#include "Light.h"

#include <vector>

class Shader
{
public:

    enum ShaderType
    {
        VertexShader,
        FragmentShader,
        GeometryShader,
        ComputeShader,
        TessCtrlShader,
        TessEvalShader        
    };

    struct ShaderInfo
    {
        ShaderInfo(GLuint id, ShaderType type) 
            :_id(id), _type(type) 
        {}

        GLuint _id;
        ShaderType _type;
    };

    Shader();
    virtual ~Shader();

    virtual GLRESULT Load();
    virtual void Unload();

    void Bind()
    {
        glUseProgram(_program);
    }
    void UnBind()
    {
        glUseProgram(0);
    }

    GLuint BindUniformBlock(const char *name, GLuint binding)
    {
        GLuint index = glGetUniformBlockIndex(_program, name);
        if (index != -1)
        {
            glUniformBlockBinding(_program, index, binding);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamInt(const char *name, int val0)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform1i(param, val0);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamInt2(const char *name, int val0, int val1)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform2i(param, val0, val1);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamInt3(const char *name, int val0, int val1, int val2)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform3i(param, val0, val1, val2);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamInt4(const char *name, int val0, int val1, int val2, int val3)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform4i(param, val0, val1, val2, val3);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamFloat(const char *name, float val0)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform1f(param, val0);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamFloat2(const char *name, float val0, float val1)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform2f(param, val0, val1);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamFloat3(const char *name, float val0, float val1, float val2)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform3f(param, val0, val1, val2);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamFloat4(const char *name, float val0, float val1, float val2, float val3)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform4f(param, val0, val1, val2, val3);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamDouble(const char *name, double val0)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform1d(param, val0);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamDouble2(const char *name, double val0, double val1)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform2d(param, val0, val1);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamDouble3(const char *name, double val0, double val1, double val2)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform3d(param, val0, val1, val2);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamDouble4(const char *name, double val0, double val1, double val2, double val3)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniform4d(param, val0, val1, val2, val3);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamMat3f(const char *name, const float* value)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniformMatrix3fv(param, 1, GL_FALSE, value);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamMat3d(const char *name, const double* value)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniformMatrix3dv(param, 1, GL_FALSE, value);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamMat4f(const char *name, const float* value)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniformMatrix4fv(param, 1, GL_FALSE, value);
            return GLOK;
        }
        return GLFAIL;
    }

    GLRESULT SetParamMat4d(const char *name, const double* value)
    {
        GLuint param = glGetUniformLocation(_program, name);
        if (param != -1)
        {
            glUniformMatrix4dv(param, 1, GL_FALSE, value);
            return GLOK;
        }
        return GLFAIL;
    }

protected:

    GLRESULT LoadShader(const char *path, ShaderType type);
    GLRESULT LoadShader(const char *data, int len, ShaderType type);

private:

    static GLenum GLShaderType(ShaderType type)
    {
        switch (type)
        {
        case VertexShader:
            return GL_VERTEX_SHADER;
        case FragmentShader:
            return GL_FRAGMENT_SHADER;
        case GeometryShader:
            return GL_GEOMETRY_SHADER;
        case TessCtrlShader:
            return GL_TESS_CONTROL_SHADER;
        case TessEvalShader:
            return GL_TESS_EVALUATION_SHADER;
        case ComputeShader:
            return GL_COMPUTE_SHADER;
        }
    }

    static GLRESULT CheckShaderCompile(GLuint shader)
    {
        GLint success;
        GLchar infoLog[0x400];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            DEBUG_LOG("Error: Shader compile failed.\n");
            DEBUG_LOG(infoLog);
            Assert(success);
            return GLFAIL;
        }
        return GLOK;
    }

    static GLRESULT CheckShaderLinking(GLuint program)
    {
        GLint success;
        GLchar infoLog[0x400];        
        glGetProgramiv(program, GL_LINK_STATUS, &success);        
        if (!success)
        {
            glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
            DEBUG_LOG("Error: Program linking failed.\n");
            DEBUG_LOG(infoLog);
            Assert(success);
            return GLFAIL;
        }
        return GLOK;
    }

private:

    GLuint _program;
    std::vector<ShaderInfo> _shaders;
};

class SimpleShader : public Shader
{
    typedef Shader Base;
public:
    SimpleShader(const GLchar *vShader, const GLchar *fShader) 
    {
        Base::Load();
        LoadShader(vShader, VertexShader);
        LoadShader(fShader, FragmentShader);
    };
    SimpleShader(const GLchar *vShader, const GLchar *fShader, const GLchar* gShader)
    {
        Base::Load();
        // GeometryShader should load before FragmentShader, after VertexShader.
        LoadShader(vShader, VertexShader);
        LoadShader(gShader, GeometryShader);
        LoadShader(fShader, FragmentShader);
    };
    virtual ~SimpleShader() {};
};

#endif // __SHADER_H