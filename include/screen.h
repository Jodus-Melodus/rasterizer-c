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

typedef struct
{
    PixelColor *buffer;
    int width, height;
    float *depthBuffer;
} ScreenBuffer;

ScreenBuffer *initScreenBuffer(const int width, const int height);
int getIndex(int x, int y, const int width, const int height);
const PixelColor get(const ScreenBuffer *screen, int x, int y);
const float getDepthBuffer(const ScreenBuffer *screen, int x, int y);
int set(ScreenBuffer *screen, int x, int y, PixelColor color);
int setDepthBuffer(ScreenBuffer *screen, int x, int y, float depth);
char *displayScreenBuffer(const ScreenBuffer *screen);
int drawTriangle(ScreenBuffer *screen, Point3D *a, Point3D *b, Point3D *c, Point2D textureCoordinate1, Point2D textureCoordinate2,
                 Point2D textureCoordinate3, PixelColor *texture, const size_t textureWidth, const size_t textureHeight);
static inline int calculateBarycentricCoordinates(Point2D a, Point2D b, Point2D c, Point2D p,
                                                  float *u, float *v, float *w);
Point3D projectCoordinate(const Point3D *p, const float focalLength);
int clearScreenBuffer(ScreenBuffer *screen);
int drawModel(ScreenBuffer *screen, const Model3D *model, const float focalLength);
void freeScreenBuffer(ScreenBuffer *screen);
int rotateModel(Model3D *model, const Axis rotationAxis, const float theta);
float normalizeDepth(const float z, const float near, const float far);
static inline int interpolateTextureCoordinates(Point2D textureCoordinate1, Point2D textureCoordinate2, Point2D textureCoordinate3,
                                                float u, float v, float w, int *textureU, int *textureV, const size_t textureWidth,
                                                const size_t textureHeight);

#endif
