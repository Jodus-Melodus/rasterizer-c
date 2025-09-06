#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>
#include "types.h"
#include "vector.h"

#define WIDTH 500
#define HEIGHT 500
#define BUFFER_SIZE (WIDTH * HEIGHT)
#define X_OFFSET (WIDTH / 2)
#define Y_OFFSET (HEIGHT / 2)

static const char GRADIENT[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

typedef struct
{
    Color *buffer;
} ScreenBuffer;

void initScreenBuffer(ScreenBuffer *sb);
void freeScreenBuffer(ScreenBuffer *sb);
int getIndex(int x, int y);
const Color get(const ScreenBuffer *sb, int x, int y);
void set(ScreenBuffer *sb, int x, int y, Color color);
const char *display(const ScreenBuffer *sb);
void drawTriangle(ScreenBuffer *sb, Vector2 a, Vector2 b, Vector2 c, Color color);
int calculateBarycentricCoordinates(Vector2 p, Vector2 a, Vector2 b, Vector2 c);
#endif
