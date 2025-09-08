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

void initScreenBuffer(ScreenBuffer *sb);
void freeScreenBuffer(ScreenBuffer *sb);
int getIndex(int x, int y);
const Color get(const ScreenBuffer *sb, int x, int y);
void set(ScreenBuffer *sb, int x, int y, Color color);
char *displayScreenBuffer(const ScreenBuffer *sb);
void drawTriangle(ScreenBuffer *sb, Vector2 *a, Vector2 *b, Vector2 *c, Color color);
int calculateBarycentricCoordinates(Vector2 *p, Vector2 *a, Vector2 *b, Vector2 *c);
Vector2 projectCoordinate(const Vector3 *p, const float focalLength);
int clearScreenBuffer(ScreenBuffer *sb);
void drawModel(ScreenBuffer *sb, const Model *model, const float focalLength);

#endif
