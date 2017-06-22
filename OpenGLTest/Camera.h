/**
 * @file: Camera.h
 *
 * @desc: Camera base on euler angles,
 *        pitch is in range of -+90.0f, yaw is in range of -+180.0f
 *
 * @version: 1.0
 *
 * @date: 2017/06/03
 *
 * @auther:
 *
 */
#ifndef __CAMERA_H
#define __CAMERA_H

#include "GLImp.h"

class Camera
{
public:

    Camera(float width, float height, float fov = 45.0, float n=0.1, float f=100.0);
    ~Camera();

    void SetFov(float fov) 
    {
        Saturate(fov, 30.0f, 60.0f);
        _fov = fov;
    }
    float GetFov() { return _fov; }

    const Vector3 &Look() { return _look; }
    const Vector3 &Up() { return _up; }
    const Vector3 &Aside() { return _aside; }
    const Vector3 &Position() { return _eye; }

    void MoveFoward();
    void MoveBack();
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();

    void MoveMouse(float x, float y);

    Matrix4 View();
    Matrix4 Perspective();

    void LookAt(const Vector3 &eye, const Vector3 &at, const Vector3 &up);

private:
    
    Vector3 _eye;
    Vector3 _at;
    Vector3 _ups;
    
    Vector3 _aside;
    Vector3 _up;
    Vector3 _look;

    float _moveSpeed;
    float _mouseSpeed;

    float _xLast;
    float _yLast;

    bool _firstMouse;

    float _yaw;
    float _pitch;

    float _fov;
    float _aspect;
    float _near;
    float _far;
};

#endif // __CAMERA_H