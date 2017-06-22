#ifndef __MATH_H
#define __MATH_H

// GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

typedef glm::vec2 Vector2;
typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;

typedef glm::mat2 Matrix2;
typedef glm::mat3 Matrix3;
typedef glm::mat4 Matrix4;

template <class Type>
inline void Saturate(Type &val, Type min, Type max)
{
    val = val > max ? max : val;
    val = val < min ? min : val;
}

#endif // __MATH_H
