// OpenGLTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "OpenGLTest.h"
#include "Camera.h"
#include "Render.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <Windows.h>

using namespace std;

bool InitGlfw(void);
bool InitGlew(void);
bool InitGlConfig(void);
void Simulate(GLFWwindow* window);
void ShowGLVersion(void);
void DoMovement();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const int GWidth = 800;
const int GHeight = 600;

Camera *GCamera;
SimpleRender *GSimpleRender;
LightRender *GLightRender;
SimpleModelRender *GSimpleModelRender;
SimpleSceneRender *GSimpleSceneRender;
SimpleFrameRender *GSimpleFrameRender;
SkyBoxRender *GSkyBoxRender;
ModifyShapeRender *GModifyShapeRender;
InstanceRender *GInstanceRender;

#ifdef _WINMAIN
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    LPSTR lpCmd = GetCommandLineA();
    int argc = 1;
    char* argv[1] = {lpCmd};
#else
int main(int argc, char* argv[])
{
#endif
    InitGlfw();

    GLFWwindow *window = glfwCreateWindow(GWidth, GHeight, "Graphic Demo", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "Error call glfwCreateWindow.\n" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // must call here, ohterwise errMsg "Missing GL version"
    InitGlew();
    InitGlConfig();

    // init camera
    GCamera = new Camera(GWidth, GHeight);
    Vector3 eye = Vector3(0.0f, 0.0f, 3.0f);
    Vector3 dir = Vector3(0.0f, 0.0f, -1.0f);
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
    GCamera->LookAt(eye, eye+dir, up);

    // init renders
    GSimpleRender = new SimpleRender();
    GSimpleRender->Load();

    GLightRender = new LightRender();
    GLightRender->Load();

    //GSimpleModelRender = new SimpleModelRender();
    //GSimpleModelRender->Load();

    GSimpleSceneRender = new SimpleSceneRender();
    GSimpleSceneRender->Load();

    GSimpleFrameRender = new SimpleFrameRender();
    GSimpleFrameRender->Load();

    GSkyBoxRender = new SkyBoxRender();
    GSkyBoxRender->Load();

    GModifyShapeRender = new ModifyShapeRender();
    GModifyShapeRender->Load();

    GInstanceRender = new InstanceRender();
    GInstanceRender->Load();

    // auto memory release
    std::unique_ptr<Camera> pCamera(GCamera);
    std::unique_ptr<SimpleRender> pSimpleRender(GSimpleRender);
    std::unique_ptr<LightRender> pLightRender(GLightRender);
    //std::unique_ptr<SimpleModelRender> pSimpleModelRender(GSimpleModelRender);
    std::unique_ptr<SimpleSceneRender> pSimpleSceneRender(GSimpleSceneRender);
    std::unique_ptr<SimpleFrameRender> pSimpleFrameRender(GSimpleFrameRender);
    std::unique_ptr<SkyBoxRender> pSkyBoxRender(GSkyBoxRender);
    std::unique_ptr<ModifyShapeRender> pModifyShapeRender(GModifyShapeRender);
    std::unique_ptr<InstanceRender> pInstanceRender(GInstanceRender);

    // setup keyboard, mouse callback
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        DoMovement();
        Simulate(window);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 1;
}

bool InitGlfw(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    return true;
}

bool InitGlew(void)
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        const GLubyte* errMsg = glewGetErrorString(err);
        cout << "Failed to initialize GLEW" << endl;
        return false;
    }
    return true;
}

bool InitGlConfig(void)
{
    glViewport(0, 0, GWidth, GHeight);
    
    // set back cull mode
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // enable depth test, default func is GL_LESS
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonOffset(0.0, 1.0);

    return true;
}

void Simulate(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    //GSimpleRender->Simulate(0.0f);
    //GLightRender->Simulate(0.0f);
    //GSimpleModelRender->Simulate(0.0f);
    //GSimpleSceneRender->Simulate(0.0f);
    //GSimpleFrameRender->Simulate(0.0f);
    //GSkyBoxRender->Simulate(0.0f);
    //GModifyShapeRender->Simulate(0.0f);
    GInstanceRender->Simulate(0.0f);

    glFlush();
    Sleep(10);
}

void ShowGLVersion(void)
{
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* identify = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("OpenGL Vendor：%s\n", vendor);
    printf("Render Identify：%s\n", identify);
    printf("OpenGL Version：%s\n", version);
}

char GKeys[GLFW_KEY_LAST] = { 0 };

void DoMovement()
{
    // Camera controls
    if (GKeys[GLFW_KEY_W])
        GCamera->MoveFoward();
    if (GKeys[GLFW_KEY_S])
        GCamera->MoveBack();
    if (GKeys[GLFW_KEY_A])
        GCamera->MoveLeft();
    if (GKeys[GLFW_KEY_D])
        GCamera->MoveRight();
    if (GKeys[GLFW_KEY_Q])
        GCamera->MoveUp();
    if (GKeys[GLFW_KEY_E])
        GCamera->MoveDown();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
        GKeys[key] = true;
    else if (action == GLFW_RELEASE)
        GKeys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    GCamera->MoveMouse(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GCamera->SetFov(GCamera->GetFov() - yoffset / 10.0f);
}