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

#define WIDTH 208
#define HEIGHT 50
#define BUFFER_SIZE (WIDTH * HEIGHT)
#define X_OFFSET (WIDTH / 2)
#define Y_OFFSET (HEIGHT / 2)

typedef struct
{
    Color *buffer;
    float *depthBuffer;
} ScreenBuffer;

ScreenBuffer *initScreenBuffer();
int getIndex(int x, int y);
const Color get(const ScreenBuffer *screen, int x, int y);
const float getDepthBuffer(const ScreenBuffer *screen, int x, int y);
int set(ScreenBuffer *screen, int x, int y, Color color);
int setDepthBuffer(ScreenBuffer *screen, int x, int y, float depth);
char *displayScreenBuffer(const ScreenBuffer *screen);
int drawTriangle(ScreenBuffer *screen, Vector3 *a, Vector3 *b, Vector3 *c, Vector2 textureCoordinate1, Vector2 textureCoordinate2,
                 Vector2 textureCoordinate3, Color *texture, const size_t textureWidth, const size_t textureHeight);
static inline int calculateBarycentricCoordinates(Vector2 a, Vector2 b, Vector2 c, Vector2 p,
                                                  float *u, float *v, float *w);
Vector3 projectCoordinate(const Vector3 *p, const float focalLength);
int clearScreenBuffer(ScreenBuffer *screen);
int drawModel(ScreenBuffer *screen, const Model *model, const float focalLength);
void freeScreenBuffer(ScreenBuffer *screen);
int rotateModel(Model *model, const Axis rotationAxis, const float theta);
float normalizeDepth(const float z, const float near, const float far);
static inline int interpolateTextureCoordinates(Vector2 textureCoordinate1, Vector2 textureCoordinate2, Vector2 textureCoordinate3,
                                                float u, float v, float w, int *textureU, int *textureV, const size_t textureWidth,
                                                const size_t textureHeight);

#endif
