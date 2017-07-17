#include "stdafx.h"
#include "Render.h"
#include "Camera.h"

using namespace glm;

extern Camera *GCamera;
extern const int GWidth = 800;
extern const int GHeight = 600;

SimpleRender::SimpleRender() : 
    _shader(new SimpleShader("Shader\\SimpleShader.vert", "Shader\\SimpleShader.frag"))
{
}

SimpleRender::~SimpleRender()
{
    Unload();
}

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};


GLRESULT SimpleRender::Load()
{
    AttrIDS attrIds = AttrPosition | AttrTex0;
    _mesh.Load(attrIds, vertices, 5 * sizeof(GLfloat), 36);

    Texture texture;
    texture.id = LoadTexture("Resource\\box.jpg");
    texture.type = GL_TEXTURE_2D;
    texture.map = "texture_diffuse";
    texture.path = "box.jpg";
    texture.uname = "texSampler1";

    _mesh.AddTexture(texture);

    return GLOK;
}

void SimpleRender::Unload()
{
}

Vector3 cubePositions[] = {
    Vector3(0.0f, 0.0f, 0.0f),
    Vector3(2.0f, 5.0f, -15.0f),
    Vector3(-1.5f, -2.2f, -2.5f),
    Vector3(-3.8f, -2.0f, -12.3f),
    Vector3(2.4f, -0.4f, -3.5f),
    Vector3(-1.7f, 3.0f, -7.5f),
    Vector3(1.3f, -2.0f, -2.5f),
    Vector3(1.5f, 2.0f, -2.5f),
    Vector3(1.5f, 0.2f, -1.5f),
    Vector3(-1.3f, 1.0f, -1.5f)
};

void SimpleRender::Simulate(float delta)
{
    _shader->Bind();

    Matrix4 view = GCamera->View();
    Matrix4 proj = GCamera->Perspective();

    // Enable stencil test, set option always success then increce stencil value
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xff);
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    for (GLuint i = 0; i < 10; i++)
    {
        // Calculate the model matrix for each object and pass it to shader before drawing
        Matrix4 model;
        model = translate(model, cubePositions[i]);
        GLfloat angle = 20.0f * i;
        model = rotate(model, angle, Vector3(1.0f, 0.3f, 0.5f));
        Matrix4 transform = proj * view * model;
        _shader->SetParamMat4f("transform", value_ptr(transform));

        _mesh.Draw(_shader.get());
    }

    glStencilMask(0x0);
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glDisable(GL_DEPTH_TEST);

    for (GLuint i = 0; i < 10; i++)
    {
        // Calculate the model matrix for each object and pass it to shader before drawing
        Matrix4 model;
        model = translate(model, cubePositions[i]);        
        GLfloat angle = 20.0f * i;
        model = rotate(model, angle, Vector3(1.0f, 0.3f, 0.5f));
        model = scale(model, Vector3(1.1f, 1.1f, 1.1f));
        Matrix4 transform = proj * view * model;
        _shader->SetParamMat4f("transform", value_ptr(transform));

        _mesh.Draw(_shader.get());
    }

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);

    _shader->UnBind();   
}

LightRender::LightRender() : 
    _shader(new SimpleShader("Shader\\LightVShader.vert", "Shader\\LightFShader.frag"))
{
}

LightRender::~LightRender()
{
    Unload();
}

GLfloat verticesN[] = {
    // Positions          // Normals           // Texture Coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

GLRESULT LightRender::Load()
{
    AttrIDS attrIds = AttrPosition | AttrNormal | AttrTex0;
    _mesh.Load(attrIds, verticesN, 8 * sizeof(GLfloat), 36);

    Texture diffuse_texture;
    Texture specular_texture;

    diffuse_texture.id = LoadTexture("Resource\\diffuseMap.png");
    diffuse_texture.type = GL_TEXTURE_2D;
    diffuse_texture.map = "texture_diffuse";
    diffuse_texture.path = "diffuseMap.png";
    diffuse_texture.uname = "texSampler1";

    specular_texture.id = LoadTexture("Resource\\specularMap.png");
    specular_texture.type = GL_TEXTURE_2D;
    specular_texture.map = "texture_specular";
    specular_texture.path = "specularMap.png";
    specular_texture.uname = "texSampler2";

    _mesh.AddTexture(diffuse_texture);
    _mesh.AddTexture(specular_texture);

    return GLOK;
}

void LightRender::Unload()
{
}

void LightRender::Simulate(float delta)
{
    _shader->Bind();

    // set material and colors
    _shader->SetParamFloat4("material.emission", 0.0f, 0.0f, 0.0f, 0.0f);
    _shader->SetParamFloat4("material.diffuse", 0.5f, 0.5f, 0.5f, 0.5f);
    _shader->SetParamFloat4("material.ambient", 0.1f, 0.1f, 0.1f, 0.1f);
    _shader->SetParamFloat4("material.specular", 0.8f, 0.8f, 0.8f, 0.8f);
    _shader->SetParamFloat4("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);
    _shader->SetParamFloat4("objectColor", 1.0f, 0.5f, 0.31f, 1.0f);

    // set light option

    Vector3 lightPos(1.0f, 1.2f, 2.0f);
    Vector3 viewPos = GCamera->Position();
    Vector3 viewDir = GCamera->Look();
    _shader->SetParamFloat3("viewPos", viewPos.x, viewPos.y, viewPos.z);

    // direction light
    _shader->SetParamFloat3("dirLight.direction", 1.0f, 0.0f, 1.0f);

    _shader->SetParamFloat4("dirLight.diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    _shader->SetParamFloat4("dirLight.ambient", 1.0f, 1.0f, 1.0f, 1.0f);
    _shader->SetParamFloat4("dirLight.specular", 1.0f, 1.0f, 1.0f, 1.0f);

    // point light
    _shader->SetParamFloat3("pointLight.position", lightPos.x, lightPos.y, lightPos.z);

    _shader->SetParamFloat4("pointLight.diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    _shader->SetParamFloat4("pointLight.ambient", 1.0f, 1.0f, 1.0f, 1.0f);
    _shader->SetParamFloat4("pointLight.specular", 1.0f, 1.0f, 1.0f, 1.0f);

    _shader->SetParamFloat("pointLight.constant", 1.0f);
    _shader->SetParamFloat("pointLight.linear", 0.14f);
    _shader->SetParamFloat("pointLight.quadratic", 0.07f);

    // spot light
    _shader->SetParamFloat3("spotLight.position", viewPos.x, viewPos.y, viewPos.z);
    _shader->SetParamFloat3("spotLight.direction", -viewDir.x, -viewDir.y, -viewDir.z);

    _shader->SetParamFloat4("spotLight.diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    _shader->SetParamFloat4("spotLight.ambient", 1.0f, 1.0f, 1.0f, 1.0f);
    _shader->SetParamFloat4("spotLight.specular", 1.0f, 1.0f, 1.0f, 1.0f);

    _shader->SetParamFloat("spotLight.cutOff", cos(TO_RADIAN(12.5f)));
    _shader->SetParamFloat("spotLight.outerCutOff", cos(TO_RADIAN(17.5f)));
    _shader->SetParamFloat("spotLight.constant", 1.0f);
    _shader->SetParamFloat("spotLight.linear", 0.022f);
    _shader->SetParamFloat("spotLight.quadratic", 0.0019f);

    // set transform matrix
    Matrix4 view = GCamera->View();
    Matrix4 proj = GCamera->Perspective();

    _shader->SetParamMat4f("view", value_ptr(view));

    for (GLuint i = 0; i < 10; i++)
    {
        // Calculate the model matrix for each object and pass it to shader before drawing
        Matrix4 model;
        model = translate(model, cubePositions[i]);
        GLfloat angle = 20.0f * i;
        model = rotate(model, angle, Vector3(1.0f, 0.3f, 0.5f));
        Matrix4 transform = proj * view * model;
        _shader->SetParamMat4f("model", value_ptr(model));
        _shader->SetParamMat4f("transform", value_ptr(transform));

        _mesh.Draw(_shader.get());
    }

    _shader->UnBind();
}

SimpleModelRender::SimpleModelRender() : 
    _modelShader(new SimpleShader("Shader\\SimpleModelShader.vert", "Shader\\SimpleModelShader.frag")) , 
    _hairShader(new SimpleShader("Shader\\SimpleModelHair.vert", "Shader\\SimpleModelHair.frag", "Shader\\SimpleModelHair.geom")) , 
    _model(new Model("Resource\\Model\\nanosuit\\nanosuit.obj"))
{

}

SimpleModelRender::~SimpleModelRender()
{
    Unload();
}

GLRESULT SimpleModelRender::Load()
{
    return GLOK;
}

void SimpleModelRender::Unload()
{
}

void SimpleModelRender::Simulate(float delta)
{
    // draw model first
    _modelShader->Bind();

    // set transform matrix
    Matrix4 view = GCamera->View();
    Matrix4 proj = GCamera->Perspective();

    // adjust model position and scale
    Matrix4 model;
    model = translate(model, Vector3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
    model = scale(model, Vector3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down

    Matrix4 transform = proj * view * model;
    _modelShader->SetParamMat4f("model", value_ptr(model));
    _modelShader->SetParamMat4f("transform", value_ptr(transform));

    Vector3 lightPos(1.0f, 1.2f, 2.0f);
    Vector3 viewPos = GCamera->Position();
    _modelShader->SetParamFloat3("viewPos", viewPos.x, viewPos.y, viewPos.z);

    // direction light
    _modelShader->SetParamFloat3("dirLight.direction", 1.0f, 1.0f, 1.0f);

    _modelShader->SetParamFloat4("dirLight.diffuse", 0.4f, 0.4f, 0.4f, 0.4f);
    _modelShader->SetParamFloat4("dirLight.ambient", 0.1f, 0.1f, 0.1f, 0.1f);
    _modelShader->SetParamFloat4("dirLight.specular", 0.2f, 0.2f, 0.2f, 0.2f);

    _model->Draw(_modelShader.get());

    _modelShader->UnBind();

    // draw model vertex hair
    _hairShader->Bind();

    _hairShader->SetParamMat4f("transform", value_ptr(transform));

    _model->Draw(_hairShader.get());

    _hairShader->UnBind();
}

SimpleSceneRender::SimpleSceneRender() : 
    _sceneShader(new SimpleShader("Shader\\SimpleShader.vert", "Shader\\SimpleShader.frag")) ,
    //_sceneShader(new SimpleShader("Shader\\ShowDepth.vert", "Shader\\ShowDepth.frag")),
    _alphaShader(new SimpleShader("Shader\\AlphaVShader.vert", "Shader\\AlphaFShader.frag"))
{
}

SimpleSceneRender::~SimpleSceneRender()
{
    Unload();
}

GLfloat cubeVertices[] = {
    // Positions          // Texture Coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};
GLfloat planeVertices[] = {
    // Positions          // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
    5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

    5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
    5.0f, -0.5f, -5.0f, 2.0f, 2.0f
};
GLfloat transparentVertices[] = {
    // Positions         // Texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
    1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
    1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.5f, 0.0f, 1.0f, 0.0f
};

GLRESULT SimpleSceneRender::Load()
{
    AttrIDS attrIds = AttrPosition | AttrTex0;
    _plane.Load(attrIds, planeVertices, 5 * sizeof(GLfloat), 6);
    _cube.Load(attrIds, cubeVertices, 5 * sizeof(GLfloat), 36);
    _board.Load(attrIds, transparentVertices, 5 * sizeof(GLfloat), 6);

    // load plane texture
    Texture texturePlane;
    texturePlane.id = LoadTexture("Resource\\plane.jpg");
    texturePlane.type = GL_TEXTURE_2D;
    texturePlane.map = "texture_diffuse";
    texturePlane.path = "plane.jpg";
    texturePlane.uname = "texSampler1";

    _plane.AddTexture(texturePlane);

    // load cube texture
    Texture textureCube;
    textureCube.id = LoadTexture("Resource\\diffuseMap.png");
    textureCube.type = GL_TEXTURE_2D;
    textureCube.map = "texture_diffuse";
    textureCube.path = "diffuseMap.png";
    textureCube.uname = "texSampler1";

    _cube.AddTexture(textureCube);

    // load board texture
    Texture textureBoard;
    textureBoard.id = LoadTexture("Resource\\grass.png", true);
    textureBoard.type = GL_TEXTURE_2D;
    textureBoard.map = "texture_diffuse";
    textureBoard.path = "grass.png";
    textureBoard.uname = "texSampler1";

    _board.AddTexture(textureBoard);

    return GLOK;
}

void SimpleSceneRender::Unload()
{

}

void SimpleSceneRender::Simulate(float delta)
{
    // draw cube twice and plane with normal shader
    _sceneShader->Bind();

    Matrix4 model;
    model = translate(model, Vector3(-1.0f, 0.0f, -1.0f));
    Matrix4 view = GCamera->View();
    Matrix4 proj = GCamera->Perspective();

    Matrix4 transform = proj * view * model;
    _sceneShader->SetParamMat4f("transform", value_ptr(transform));
    _cube.Draw(_sceneShader.get());

    model = translate(model, Vector3(2.0f, 0.0f, 0.0f));
    transform = proj * view * model;
    _sceneShader->SetParamMat4f("transform", value_ptr(transform));
    _cube.Draw(_sceneShader.get());

    model = Matrix4();
    transform = proj * view * model;
    _sceneShader->SetParamMat4f("transform", value_ptr(transform));
    _plane.Draw(_sceneShader.get());

    _sceneShader->UnBind();

    // draw board with alpha enable
    _alphaShader->Bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    vector<Vector3> vegetation;
    vegetation.push_back(Vector3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(Vector3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(Vector3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(Vector3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(Vector3(0.5f, 0.0f, -0.6f));

    for (GLuint i = 0; i < vegetation.size(); i++)
    {
        model = Matrix4();
        model = translate(model, vegetation[i]);
        transform = proj * view * model;
        _alphaShader->SetParamMat4f("transform", value_ptr(transform));
        _board.Draw(_alphaShader.get());
    }

    glDisable(GL_BLEND);

    _alphaShader->UnBind();
}

SimpleFrameRender::SimpleFrameRender() : 
    _sceneShader(new SimpleShader("Shader\\SimpleShader.vert", "Shader\\SimpleShader.frag")) ,
    _frameShader(new SimpleShader("Shader\\FrameVShader.vert", "Shader\\FrameFShader.frag"))
{

}

SimpleFrameRender::~SimpleFrameRender()
{
    Unload();
}

GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // Positions   // TexCoords
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f
};

GLRESULT SimpleFrameRender::Load()
{
    AttrIDS attrIds = AttrPosition | AttrTex0;
    _plane.Load(attrIds, planeVertices, 5 * sizeof(GLfloat), 6);
    _cube.Load(attrIds, cubeVertices, 5 * sizeof(GLfloat), 36);
    _quad.Load(attrIds, quadVertices, 5 * sizeof(GLfloat), 6);

    // Load plane texture
    Texture texturePlane;
    texturePlane.id = LoadTexture("Resource\\plane.jpg");
    texturePlane.type = GL_TEXTURE_2D;
    texturePlane.map = "texture_diffuse";
    texturePlane.path = "plane.jpg";
    texturePlane.uname = "texSampler1";

    _plane.AddTexture(texturePlane);

    // Load cube texture
    Texture textureCube;
    textureCube.id = LoadTexture("Resource\\diffuseMap.png");
    textureCube.type = GL_TEXTURE_2D;
    textureCube.map = "texture_diffuse";
    textureCube.path = "diffuseMap.png";
    textureCube.uname = "texSampler1";

    _cube.AddTexture(textureCube);

    // Create frame buffer
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Create texture as frame pixel buffer
    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GWidth, GHeight, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bind frame buffer with texture2D
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex, 0);

    // Create render buffer using as depth-stencil attachment
    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GWidth, GHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Bind frame buffer with render buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Assert(0);
        DEBUG_LOG("Error frame buffer.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Load quad texture
    Texture textureScree;
    textureScree.id = _tex;
    textureScree.type = GL_TEXTURE_2D;
    textureScree.map = "texture_diffuse";
    textureScree.uname = "screenTexture";

    _quad.AddTexture(textureScree);

    return GLOK;
}

void SimpleFrameRender::Unload()
{
    glDeleteRenderbuffers(1, &_rbo);
    glDeleteFramebuffers(1, &_fbo);
}

void SimpleFrameRender::Simulate(float delta)
{
    // Draw sence
    _sceneShader->Bind();

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    // Clear all attached buffers        
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    Matrix4 model;
    model = translate(model, Vector3(-1.0f, 0.0f, -1.0f));
    Matrix4 view = GCamera->View();
    Matrix4 proj = GCamera->Perspective();

    Matrix4 transform = proj * view * model;
    _sceneShader->SetParamMat4f("transform", value_ptr(transform));
    _cube.Draw(_sceneShader.get());

    model = translate(model, Vector3(2.0f, 0.0f, 0.0f));
    transform = proj * view * model;
    _sceneShader->SetParamMat4f("transform", value_ptr(transform));
    _cube.Draw(_sceneShader.get());

    model = Matrix4();
    transform = proj * view * model;
    _sceneShader->SetParamMat4f("transform", value_ptr(transform));
    _plane.Draw(_sceneShader.get());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    _sceneShader->UnBind();

    // Draw a rectangle using framebuffer as texture
    _frameShader->Bind();
    _quad.Draw(_frameShader.get());
    _frameShader->UnBind();
}

SkyBoxRender::SkyBoxRender() : 
    _sceneShader(new SimpleShader("Shader\\SkyBoxVShader.vert", "Shader\\SkyBoxFShader.frag"))
{
}

SkyBoxRender::~SkyBoxRender()
{
    Unload();
}

float skyboxVertices[] = {
    // positions          
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

GLRESULT SkyBoxRender::Load()
{
    // Load skybox
    _skybox.Load(AttrPosition, skyboxVertices, sizeof(GLfloat)* 3, 36);

    // Load cube texture
    vector<const char*> faces;
    faces.push_back("Resource\\skybox\\right.jpg");
    faces.push_back("Resource\\skybox\\left.jpg");
    faces.push_back("Resource\\skybox\\top.jpg");
    faces.push_back("Resource\\skybox\\bottom.jpg");
    faces.push_back("Resource\\skybox\\front.jpg");
    faces.push_back("Resource\\skybox\\back.jpg");

    Texture texCube;
    texCube.id = LoadCubeTexture(faces, false);
    texCube.type = GL_TEXTURE_CUBE_MAP;
    texCube.map = "texture_diffuse";
    texCube.uname = "cubemap";

    _skybox.AddTexture(texCube);

    // bind uniform block   
    glGenBuffers(1, &_uboMatrix);
    glBindBuffer(GL_UNIFORM_BUFFER, _uboMatrix);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix4)* 4, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    const int matIndex = 0;
    _sceneShader->BindUniformBlock("allmat", matIndex);
    glBindBufferRange(GL_UNIFORM_BUFFER, matIndex, _uboMatrix, 0, sizeof(Matrix4)* 4);

    glGenBuffers(1, &_uboColor);
    glBindBuffer(GL_UNIFORM_BUFFER, _uboColor);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Vector4)* 5, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    const int colorIndex = 1;
    _sceneShader->BindUniformBlock("material", colorIndex);
    glBindBufferRange(GL_UNIFORM_BUFFER, colorIndex, _uboColor, 0, sizeof(Vector4)* 5);

    return GLOK;
}

void SkyBoxRender::Unload()
{
    glDeleteBuffers(1, &_uboMatrix);
}

void SkyBoxRender::Simulate(float delta)
{
    _sceneShader->Bind();

    glDepthFunc(GL_EQUAL);

    Matrix4 model;
    model = scale(model, Vector3(50.0f, 50.0f, 50.0f));
    Matrix4 view = GCamera->View();
    Matrix4 proj = GCamera->Perspective();

    Matrix4 trans = proj * view * model;
    //_sceneShader->SetParamMat4f("transform", value_ptr(transform));

    glBindBuffer(GL_UNIFORM_BUFFER, _uboMatrix);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4)*0, sizeof(Matrix4), value_ptr(model));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4)*1, sizeof(Matrix4), value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4)*2, sizeof(Matrix4), value_ptr(proj));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4)*3, sizeof(Matrix4), value_ptr(trans));
    

    Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    glBindBuffer(GL_UNIFORM_BUFFER, _uboColor);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vector4)* 0, sizeof(Vector4), value_ptr(color));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vector4)* 1, sizeof(Vector4), value_ptr(color));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vector4)* 2, sizeof(Vector4), value_ptr(color));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vector4)* 3, sizeof(Vector4), value_ptr(color));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Vector4)* 4, sizeof(Vector4), value_ptr(color));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    _skybox.Draw(_sceneShader.get());

    glDepthFunc(GL_LESS);

    _sceneShader->UnBind();
}

ModifyShapeRender::ModifyShapeRender() : 
    _shapeShader(new SimpleShader(
        "Shader\\ShapeModify.vert", 
        "Shader\\ShapeModify.frag",
        "Shader\\ShapeModify.geom"))
{
}

ModifyShapeRender::~ModifyShapeRender()
{
    Unload();
}

const GLfloat pointVertices[] = {
    // position         // color
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
};

GLRESULT ModifyShapeRender::Load()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertices), &pointVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);

    return GLOK;
}

void ModifyShapeRender::Unload()
{
    glDeleteBuffers(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void ModifyShapeRender::Simulate(float delta)
{
    _shapeShader->Bind();

    glBindVertexArray(_vao);
    glDrawArrays(GL_POINTS, 0, 4);
    glBindVertexArray(0);

    _shapeShader->UnBind();
}

InstanceRender::InstanceRender() : 
    _shader(new SimpleShader("Shader\\InstanceShader.vert", "Shader\\InstanceShader.frag"))
{
}

InstanceRender::~InstanceRender()
{
    Unload();
}

const GLfloat instanceVertices[] = {
    // position     // color
    -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
    0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

    -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
    0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
    0.05f, 0.05f, 0.0f, 1.0f, 1.0f
};

GLRESULT InstanceRender::Load()
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instanceVertices), &instanceVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);

    Vector2 *offsets = new Vector2[100];
    int index = 0;
    for (int i = -10; i < 10; i+=2)
    {
        for (int j = -10; j < 10; j+=2)
        {
            offsets[index++] = Vector2((float)i / 10.0f, (float)j / 10.0f);
        }
    }

    _shader->Bind();
    _shader->SetParamFloatArray<2>("offset", 100, value_ptr(offsets[0]));
    _shader->UnBind();

    delete[] offsets;

    return GLOK;
}

void InstanceRender::Unload()
{
    glDeleteBuffers(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}

void InstanceRender::Simulate(float delta)
{
    _shader->Bind();

    glBindVertexArray(_vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
    glBindVertexArray(0);

    _shader->UnBind();
}

MSAARender::MSAARender() :
    _shader(new SimpleShader("Shader\\MSAAShader.vert", "Shader\\MSAAShader.frag")),
    _fbo(0),
    _rbo(0)
{
}

MSAARender::~MSAARender()
{
    Unload();
}

GLRESULT MSAARender::Load()
{
    _cude.Load(AttrPosition, skyboxVertices, sizeof(GLfloat)* 3, 36);

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _tex);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, GWidth, GHeight, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _tex, 0);

    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, GWidth, GHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return GLOK;
}

void MSAARender::Unload()
{
    glDeleteFramebuffers(1, &_fbo);
    glDeleteRenderbuffers(1, &_rbo);
    glDeleteTextures(1, &_tex);
}

void MSAARender::Simulate(float delta)
{
    // draw cube
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader->Bind();

    Matrix4 model;
    Matrix4 view = GCamera->View();
    Matrix4 proj = GCamera->Perspective();

    Matrix4 trans = proj * view * model;
    _shader->SetParamMat4f("trans", value_ptr(trans));

    _cude.Draw(_shader.get());

    _shader->UnBind();

    // blit framebuffer to screen
    glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, GWidth, GHeight, 0, 0, GWidth, GHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);    
}

ShadowRender::ShadowRender() :
    _sceneShader(new SimpleShader("Shader\\shadowScene.vert", "Shader\\shadowScene.frag")),
    _depthShader(new SimpleShader("Shader\\ShowDepth.vert", "Shader\\ShowDepth.frag")),
    _shadowMapShader(new SimpleShader("", ""))
{}

ShadowRender::~ShadowRender()
{
    Unload();
}

GLfloat floorVertices[] = {
    // Positions   // TexCoords
    -10.0f, 0.0f, -10.0f, 0.0f, 1.0f,
    -10.0f, 0.0f, 10.0f, 0.0f, 0.0f,
    10.0f, 0.0f, 10.0f, 1.0f, 0.0f,

    -10.0f, 0.0f, -10.0f, 0.0f, 1.0f,
    10.0f, 0.0f, 10.0f, 1.0f, 0.0f,
    10.0f, 0.0f, -10.0f, 1.0f, 1.0f
};

const int depthMapWidth = 1024;
const int depthMapHeight = 1024;

GLRESULT ShadowRender::Load()
{
    // Load box and floor mesh.
    AttrIDS attrIds = AttrPosition | AttrTex0;
    _spot.Load(attrIds, cubeVertices, 5 * sizeof(GLfloat), 36);
    _box.Load(attrIds, cubeVertices, 5 * sizeof(GLfloat), 36);
    _floor.Load(attrIds, floorVertices, 5 * sizeof(GLfloat), 6);

    // Load mesh texture
    Texture texBox;
    texBox.id = LoadTexture("Resource\\box.jpg", false);
    texBox.type = GL_TEXTURE_2D;
    texBox.map = "texture_diffuse";
    texBox.uname = "texSampler";

    _box.AddTexture(texBox);

    Texture texFloor;
    texFloor.id = LoadTexture("Resource\\floor.jpg", false);
    texFloor.type = GL_TEXTURE_2D;
    texFloor.map = "texture_diffuse";
    texFloor.uname = "texSampler";

    _floor.AddTexture(texFloor);

    // Load frameBuffer, bind frameBuffer with texture
    // Output texture as a depth map
    // Must call glViewPort set screen width/height same as depth texture width/height.
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glGenTextures(1, &_depthMap);
    glBindTexture(GL_TEXTURE_2D, _depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapWidth, depthMapHeight,
        0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return GLOK;
}

void ShadowRender::Unload()
{
    glDeleteTextures(1, &_depthMap);
    glDeleteFramebuffers(1, &_fbo);
}

void ShadowRender::Simulate(float delta)
{
    glViewport(0, 0, depthMapWidth, depthMapHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    _depthShader->Bind();

    // Draw floor
    Matrix4 model = translate(Matrix4(), Vector3(0.0f, -1.0f, 0.0f));
    Matrix4 view = GCamera->View();
    Matrix4 proj = GCamera->Perspective();
    Matrix4 trans = proj * view * model;
    _depthShader->SetParamMat4f("transform", value_ptr(trans));
    _floor.Draw(_depthShader.get());

    // Draw left box
    model = translate(Matrix4(), Vector3(-2.0f, 1.0f, 0.0f));
    trans = proj * view * model;
    _depthShader->SetParamMat4f("transform", value_ptr(trans));
    _box.Draw(_depthShader.get());

    // Draw right box
    model = translate(Matrix4(), Vector3(2.0f, 1.0f, 0.0f));
    model = rotate(model, TO_RADIAN(30.0f), Vector3(1.0f, 1.0f, 1.0f));
    trans = proj * view * model;
    _depthShader->SetParamMat4f("transform", value_ptr(trans));
    _box.Draw(_depthShader.get());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, GWidth, GHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shadowMapShader->Bind();
    _shadowMapShader->UnBind();

}