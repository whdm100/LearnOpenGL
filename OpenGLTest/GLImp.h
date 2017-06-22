#ifndef _GLIMP_H
#define _GLIMP_H

// GLEW Library
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // GLEW_STATIC

#include <GL/glew.h>
#pragma comment(lib, "glew32s.lib")

// GLFW Library
#include <GLFW/glfw3.h>
#ifdef _DEBUG
#pragma comment(lib, "glfwd.lib")
#else
#pragma comment(lib, "glfw.lib")
#endif //_DEBUG

// OPENGL Library
#pragma comment(lib, "opengl32.lib")

// SOIL image loading library
#include <SOIL.h>
#ifdef _DEBUG
#pragma comment(lib, "libSOILD.lib")
#else
#pragma comment(lib, "libSOILd.lib")
#endif //_DEBUG

// Assimp Library
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment(lib, "assimp-vc120-mt.lib")

// Math Library
#include "Math.h"

// Template class
#include "RefCount.hpp"

// Status define
typedef int GLRESULT;

#define GLOK (GLRESULT)0
#define GLFAIL (GLRESULT)-1

#define GL_FAILED(r) (r!=GLOK)


#ifdef _DEBUG
#define DEBUG_LOG(l) OutputDebugStringA
#define Assert(r) if (!r) __asm{int 3}
#else
#define Assert(r)
#define DEBUG_LOG(l)
#endif // _DEBUG

// Math define
#define PI 3.1415926f
#define TO_RADIAN(a) (((float)a / 180.0f) * PI)

#endif // _GLIMP_H