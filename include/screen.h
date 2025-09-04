#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>
#include "types.h"

#define WIDTH 20
#define HEIGHT 10
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

#endif
