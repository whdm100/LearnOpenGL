/**
 * @file: Render.h
 *
 * @desc: render effect classes.
 *
 * @version: 1.0
 *
 * @date: 2017/06/03
 *
 * @auther:
 *
 */

#ifndef __RENDER_H
#define __RENDER_H

#include "GLImp.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

#include <memory>

class Render
{
public:
    Render() {};
    virtual ~Render() {};

    virtual GLRESULT Load() = 0;
    virtual void Unload() = 0;

    virtual void Begin() {}
    virtual void End() {}

    virtual void Simulate(float delta) = 0;
};

class SimpleRender : public Render
{
    typedef Render Base;
public:
    SimpleRender();
    virtual ~SimpleRender();

    virtual GLRESULT Load();
    virtual void Unload();

    virtual void Simulate(float delta);

private:
    std::unique_ptr<Shader> _shader;
    Mesh _mesh;
};

class LightRender : public Render
{
    typedef Render Base;
public:
    LightRender();
    virtual ~LightRender();

    virtual GLRESULT Load();
    virtual void Unload();

    virtual void Simulate(float delta);

private:
    std::unique_ptr<Shader> _shader;
    Mesh _mesh;
};

class SimpleModelRender : public Render
{
    typedef Render Base;
public:
    SimpleModelRender();
    virtual ~SimpleModelRender();

    virtual GLRESULT Load();
    virtual void Unload();

    virtual void Simulate(float delta);

private:
    std::unique_ptr<Shader> _modelShader;
    std::unique_ptr<Shader> _hairShader;
    std::unique_ptr<Model> _model;
};

class SimpleSceneRender : public Render
{
    typedef Render Base;
public:
    SimpleSceneRender();
    virtual ~SimpleSceneRender();

    virtual GLRESULT Load();
    virtual void Unload();

    virtual void Simulate(float delta);

private:
    std::unique_ptr<Shader> _sceneShader;
    std::unique_ptr<Shader> _alphaShader;
    Mesh _plane;
    Mesh _cube;
    Mesh _board;
};

class SimpleFrameRender : public Render
{
    typedef Render Base;
public:
    SimpleFrameRender();
    virtual ~SimpleFrameRender();

    virtual GLRESULT Load();
    virtual void Unload();

    virtual void Simulate(float delta);

private:
    std::unique_ptr<Shader> _sceneShader;
    std::unique_ptr<Shader> _frameShader;
    Mesh _plane;
    Mesh _cube;
    Mesh _quad;
    GLuint _fbo;
    GLuint _rbo;
    GLuint _tex;
};

class SkyBoxRender : public Render
{
    typedef Render Base;
public:
    SkyBoxRender();
    virtual ~SkyBoxRender();

    virtual GLRESULT Load();
    virtual void Unload();

    virtual void Simulate(float delta);

private:
    std::unique_ptr<Shader> _sceneShader;
    Mesh _skybox;
    GLuint _uboMatrix;
    GLuint _uboColor;
};

class ModifyShapeRender : public Render
{
    typedef Render Base;
public:
    ModifyShapeRender();
    virtual ~ModifyShapeRender();

    virtual GLRESULT Load();
    virtual void Unload();

    virtual void Simulate(float delta);

private:
    std::unique_ptr<Shader> _shapeShader;
    GLuint _vbo;
    GLuint _vao;
};

class InstanceRender : public Render
{
    typedef Render Base;
public:
    InstanceRender();
    virtual ~InstanceRender();

    virtual GLRESULT Load();
    virtual void Unload();

    virtual void Simulate(float delta);

private:
    std::unique_ptr<Shader> _shader;
    GLuint _vao;
    GLuint _vbo;
};

#endif // __RENDER_H