#include "types.h"

Matrix4 perspectiveMatrix4(float fov, float aspect, float near, float far)
{
    float f = 1.0f / tanf(fov * 0.5f);
    Matrix4 mat = {0};
    mat.m[0][0] = f / aspect;
    mat.m[1][1] = f;
    mat.m[2][2] = (far + near) / (near - far);
    mat.m[2][3] = (2 * far * near) / (near - far);
    mat.m[3][2] = -1.0f;
    return mat;
}

Vector4 TransformVector4(Matrix4 m, Vector4 v)
{
    Vector4 result = {0};
    for (int i = 0; i < 4; i++)
    {
        result.x += m.m[i][0] * v.x;
        result.y += m.m[i][1] * v.y;
        result.z += m.m[i][2] * v.z;
        result.w += m.m[i][3] * v.w;
    }
    return result;
}