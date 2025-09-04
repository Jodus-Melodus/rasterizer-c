#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>

#define WIDTH 1280
#define HEIGHT 720
#define BUFFER_SIZE (WIDTH * HEIGHT)
#define X_OFFSET (WIDTH / 2)
#define Y_OFFSET (HEIGHT / 2)

static const char GRADIENT[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

struct ScreenBuffer
{
    unsigned char *buffer;
};

void initScreenBuffer(struct ScreenBuffer *sb);
void freeScreenBuffer(struct ScreenBuffer *sb);
int getIndex(int x, int y);
const unsigned char get(const struct ScreenBuffer *sb, int x, int y);
void set(struct ScreenBuffer *sb, int x, int y, char value);
const char *display(struct ScreenBuffer *sb);

#endif
