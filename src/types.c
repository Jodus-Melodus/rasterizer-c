#include "types.h"

char asChar(PixelColor color)
{
    float gray = (0.299f * color.r + 0.587f * color.g + 0.114f * color.b) / 255.0f;
    if (gray < 0.0f)
        gray = 0.0f;
    if (gray > 1.0f)
        gray = 1.0f;

    size_t n = sizeof(GRADIENT) / sizeof(GRADIENT[0]);
    size_t index = (size_t)roundf(gray * (float)(n - 1));
    return GRADIENT[index];
}

RasterMatrix4 perspective(float fov, float aspect, float near, float far)
{
    float f = 1.0f / tanf(fov * 0.5f);
    RasterMatrix4 mat = {0};
    mat.m[0][0] = f / aspect;
    mat.m[1][1] = f;
    mat.m[2][2] = (far + near) / (near - far);
    mat.m[2][3] = (2 * far * near) / (near - far);
    mat.m[3][2] = -1.0f;
    return mat;
}

RasterVector4 mat4_mul_vec4(RasterMatrix4 m, RasterVector4 v)
{
    RasterVector4 result = {0};
    for (int i = 0; i < 4; i++)
    {
        result.x += m.m[i][0] * v.x;
        result.y += m.m[i][1] * v.y;
        result.z += m.m[i][2] * v.z;
        result.w += m.m[i][3] * v.w;
    }
    return result;
}