#include "stdafx.h"
#include "Camera.h"

using namespace glm;

Camera::Camera(float width, float height, float fov, float n, float f)
    : _moveSpeed(0.1f)
    , _mouseSpeed(0.05f)
    , _yaw(-90.0f)
    , _pitch(0.0f)
    , _fov(fov)
    , _near(n)
    , _far(f)
{
    _xLast = width / 2.0f;
    _yLast = height / 2.0f;

    _firstMouse = true;

    _aspect = width / height;

    _aside = Vector3(1.0, 0.0, 0.0);
    _up = Vector3(0.0, 1.0, 0.0);
    _look = Vector3(0.0, 0.0, 1.0);
}

Camera::~Camera()
{
}

void Camera::LookAt(const Vector3 &eye, const Vector3 &at, const Vector3 &up)
{
    _eye = eye;
    _at = at;
    _ups = up;

    _look = normalize(at - eye);
    _aside = normalize(cross(_look, up));
    _up = normalize(cross(_aside, _look));
}

void Camera::MoveFoward()
{
    _eye += _look * _moveSpeed;
}

void Camera::MoveBack()
{
    _eye -= _look * _moveSpeed;
}

void Camera::MoveUp()
{
    _eye += _up * _moveSpeed;
}

void Camera::MoveDown()
{
    _eye -= _up * _moveSpeed;
}

void Camera::MoveLeft()
{
    _eye -= _aside * _moveSpeed;
}

void Camera::MoveRight()
{
    _eye += _aside * _moveSpeed;
}

void Camera::MoveMouse(float x, float y)
{
    if (_firstMouse)
    {
        _xLast = x;
        _yLast = y;
        _firstMouse = false;
    }

    float xOffset = x - _xLast;
    float yOffset = _yLast - y;

    _xLast = x;
    _yLast = y;

    _pitch += yOffset * _mouseSpeed; 
    _yaw += xOffset * _mouseSpeed;

    Saturate(_pitch, -89.0f, 89.0f);
    Saturate(_yaw, -179.0f, 0.0f);

    float sinP = sin(TO_RADIAN(_pitch));
    float cosP = cos(TO_RADIAN(_pitch));
    float sinY = sin(TO_RADIAN(_yaw));
    float cosY = cos(TO_RADIAN(_yaw));

    _look = normalize(Vector3(cosP*cosY, sinP, cosP*sinY));
    _aside = normalize(cross(_look, _ups));
    _up = normalize(cross(_aside, _look));
}

Matrix4 Camera::View()
{
    Matrix4 view;
    view[0][0] = _aside.x;
    view[1][0] = _aside.y;
    view[2][0] = _aside.z;
    view[0][1] = _up.x;
    view[1][1] = _up.y;
    view[2][1] = _up.z;
    view[0][2] = -_look.x;
    view[1][2] = -_look.y;
    view[2][2] = -_look.z;
    view[3][0] = -glm::dot(_eye, _aside);
    view[3][1] = -glm::dot(_eye, _up);
    view[3][2] =  glm::dot(_eye, _look);
    view[3][3] = 1.0f;
    return view;
}

Matrix4 Camera::Perspective()
{
    return glm::perspective(_fov, _aspect, _near, _far);
}