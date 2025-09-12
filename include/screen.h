#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#include "types.h"
#include "model.h"

#define PI 3.14159265358979323846f

typedef struct
{
    Color *buffer;
    int width, height;
    float *depthBuffer;
} ScreenBuffer;

ScreenBuffer *initScreenBuffer(const int width, const int height);
int getIndex(int x, int y, const int width, const int height);
const Color get(const ScreenBuffer *screen, int x, int y);
const float getDepthBuffer(const ScreenBuffer *screen, int x, int y);
int set(ScreenBuffer *screen, int x, int y, Color color);
int setDepthBuffer(ScreenBuffer *screen, int x, int y, float depth);
int drawTriangle(ScreenBuffer *screen, Vector3 *a, Vector3 *b, Vector3 *c, Vector2 textureCoordinate1, Vector2 textureCoordinate2,
                 Vector2 textureCoordinate3, Color *texture, const size_t textureWidth, const size_t textureHeight);
static inline int calculateBarycentricCoordinates(Vector2 a, Vector2 b, Vector2 c, Vector2 p,
                                                  float *u, float *v, float *w);
Vector3 projectCoordinate(const Vector3 *p, RasterMatrix4 projectionMatrix);
int clearScreenBuffer(ScreenBuffer *screen);
int drawModel(ScreenBuffer *screen, const Model3D *model, const float focalLength);
void freeScreenBuffer(ScreenBuffer *screen);
int rotateModel(Model3D *model, const Axis rotationAxis, const float theta);
float normalizeDepth(const float z, const float near, const float far);
static inline int interpolateTextureCoordinates(Vector2 textureCoordinate1, Vector2 textureCoordinate2, Vector2 textureCoordinate3,
                                                float u, float v, float w, int *textureU, int *textureV, const size_t textureWidth,
                                                const size_t textureHeight);

#endif
