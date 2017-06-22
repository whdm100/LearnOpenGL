#ifndef __LIGHT_H
#define __LIGHT_H

#include "GLImp.h"

struct Material
{
    Vector4 emission;
    Vector4 diffuse;
    Vector4 ambient;
    Vector4 specular;

    float intensity;
};

struct DirectionLight
{
    Vector3 direction;
    Vector4 diffuse;
    Vector4 ambient;
    Vector4 specular;
};

struct PointLight
{
    Vector3 position;

    Vector4 diffuse;
    Vector4 ambient;
    Vector4 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    Vector3 position;
    Vector3 direction;

    Vector4 diffuse;
    Vector4 ambient;
    Vector4 specular;

    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};
#endif // __LIGHT_H