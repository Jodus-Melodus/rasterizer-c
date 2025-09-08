#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "screen.h"
#include "utils.h"

void initScreenBuffer(ScreenBuffer *sb)
{
    if (sb == NULL)
        return;
    sb->buffer = malloc(BUFFER_SIZE * sizeof(Color));
    if (sb->buffer)
        memset(sb->buffer, 0, BUFFER_SIZE * sizeof(Color));
}

void freeScreenBuffer(ScreenBuffer *sb)
{
    if (sb == NULL)
        return;
    free(sb->buffer);
    sb->buffer = NULL;
}

int getIndex(int x, int y)
{
    int indexX = x + X_OFFSET;
    int indexY = y + Y_OFFSET;

    if (indexX < 0 || indexX >= WIDTH || indexY < 0 || indexY >= HEIGHT)
        return -1;

    return WIDTH * indexY + indexX;
}

const Color get(const ScreenBuffer *sb, int x, int y)
{
    if (sb == NULL)
        return (Color){.r = 0, .g = 0, .b = 0};
    int index = getIndex(x, y);
    if (index == -1)
        return (Color){.r = 0, .g = 0, .b = 0};

    return sb->buffer[index];
}

void set(ScreenBuffer *sb, int x, int y, Color color)
{
    if (sb == NULL)
        return;
    int index = getIndex(x, y);

    if (index == -1)
        return;

    sb->buffer[index] = color;
}
char *displayScreenBuffer(const ScreenBuffer *sb)
{
    if (sb == NULL)
        return 0;

    char *result = malloc(BUFFER_SIZE + HEIGHT + 1);
    if (!result)
        return NULL;

    size_t pos = 0;

    for (int y = -Y_OFFSET; y < HEIGHT - Y_OFFSET; y++)
    {
        for (int x = -X_OFFSET; x < WIDTH - X_OFFSET; x++)
        {
            unsigned char gray = toGray(get(sb, x, y));
            int gradientIndex = gray * (sizeof(GRADIENT) - 1) / 255;
            result[pos++] = GRADIENT[gradientIndex];
        }
        result[pos++] = '\n';
    }
    result[pos] = '\0';
    return result;
}

void drawTriangle(ScreenBuffer *sb, Vector2 *a, Vector2 *b, Vector2 *c, Color color)
{
    int maxX = (int)ceilf(max(a->x, max(b->x, c->x)));
    int minX = (int)floorf(min(a->x, min(b->x, c->x)));
    int maxY = (int)ceilf(max(a->y, max(b->y, c->y)));
    int minY = (int)floorf(min(a->y, min(b->y, c->y)));

    for (int y = minY; y < maxY; y++)
    {
        for (int x = minX; x < maxX; x++)
        {
            Vector2 p = {x, y};

            if (calculateBarycentricCoordinates(&p, a, b, c))
                set(sb, x, y, color);
        }
    }
}

int calculateBarycentricCoordinates(Vector2 *p, Vector2 *a, Vector2 *b, Vector2 *c)
{
    float denominator = (b->y - c->y) * (a->x - c->x) + (c->x - b->x) * (a->y - c->y);
    float u = ((b->y - c->y) * (p->x - c->x) + (c->x - b->x) * (p->y - c->y)) / denominator;
    float v = ((c->y - a->y) * (p->x - c->x) + (a->x - c->x) * (p->y - c->y)) / denominator;
    float w = 1.0 - u - v;
    return (u >= 0.0) && (v >= 0.0) && (w >= 0.0);
}

Vector2 projectCoordinate(const Vector3 *p, const float focalLength)
{
    float denominator = focalLength + p->z;
    if (denominator == 0.0)
        denominator = 0.00001;
    float projectedX = (focalLength * p->x) / denominator;
    float projectedY = (focalLength * p->y) / denominator;
    Vector2 projected = {projectedX, projectedY};
    return projected;
}

int clearScreenBuffer(ScreenBuffer *sb)
{
    if (sb == NULL || sb->buffer == NULL)
        return 1;

    memset(sb->buffer, 0, BUFFER_SIZE * sizeof(Color));
    return 0;
}

void drawModel(ScreenBuffer *sb, const Model *model, const float focalLength)
{
    for (size_t i = 0; i < model->faceCount; i++)
    {
        size_t faceIndex1 = model->faces[i][0];
        size_t faceIndex2 = model->faces[i][1];
        size_t faceIndex3 = model->faces[i][2];

        Vector3 *vertices = model->vertices;
        Vector2 vertex1, vertex2, vertex3;
        vertex1 = projectCoordinate(&vertices[faceIndex1], focalLength);
        vertex2 = projectCoordinate(&vertices[faceIndex2], focalLength);
        vertex3 = projectCoordinate(&vertices[faceIndex3], focalLength);
        Color color;
        color.r = randColor();
        color.g = randColor();
        color.b = randColor();

        drawTriangle(sb, &vertex1, &vertex2, &vertex3, color);
    }
}
