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

struct ScreenBuffer
{
    struct Color *buffer;
};

void initScreenBuffer(struct ScreenBuffer *sb);
void freeScreenBuffer(struct ScreenBuffer *sb);
int getIndex(int x, int y);
const struct Color get(const struct ScreenBuffer *sb, int x, int y);
void set(struct ScreenBuffer *sb, int x, int y, struct Color color);
const char *display(const struct ScreenBuffer *sb);
void drawTriangle(struct ScreenBuffer *sb, struct Vector2 a, struct Vector2 b, struct Vector2 c, struct Color color);
int calculateBarycentricCoordinates(struct Vector2 p, struct Vector2 a, struct Vector2 b, struct Vector2 c);
#endif
