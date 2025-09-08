#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>
#include "types.h"
#include "vector.h"
#include "model.h"

#define WIDTH 208
#define HEIGHT 50
#define BUFFER_SIZE (WIDTH * HEIGHT)
#define X_OFFSET (WIDTH / 2)
#define Y_OFFSET (HEIGHT / 2)

typedef struct
{
    Color *buffer;
} ScreenBuffer;

ScreenBuffer *initScreenBuffer();
int getIndex(int x, int y);
const Color get(const ScreenBuffer *screen, int x, int y);
int set(ScreenBuffer *screen, int x, int y, Color color);
char *displayScreenBuffer(const ScreenBuffer *screen);
int drawTriangle(ScreenBuffer *screen, Vector2 *a, Vector2 *b, Vector2 *c, Color color);
int calculateBarycentricCoordinates(Vector2 *a, Vector2 *b, Vector2 *c, Vector2 *p);
Vector2 projectCoordinate(const Vector3 *p, const float focalLength);

int clearScreenBuffer(ScreenBuffer *screen);
void drawModel(ScreenBuffer *screen, const Model *model, const float focalLength);
void freeScreenBuffer(ScreenBuffer *screen);

#endif
