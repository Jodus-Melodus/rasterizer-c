#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "screen.h"
#include "utils.h"

ScreenBuffer *initScreenBuffer()
{
    ScreenBuffer *screen = malloc(sizeof(ScreenBuffer));
    if (!screen)
        return NULL;
    screen->buffer = malloc(BUFFER_SIZE * sizeof(Color));
    screen->depthBuffer = malloc(BUFFER_SIZE * sizeof(float));
    if (!screen->buffer || !screen->depthBuffer)
    {
        free(screen);
        return NULL;
    }
    memset(screen->buffer, 0, BUFFER_SIZE * sizeof(Color));
    memset(screen->depthBuffer, 0, BUFFER_SIZE * sizeof(float));

    return screen;
}

int getIndex(int x, int y)
{
    int indexX = x + X_OFFSET;
    int indexY = y + Y_OFFSET;

    if (indexX < 0 || indexX >= WIDTH || indexY < 0 || indexY >= HEIGHT)
        return -1;

    return WIDTH * indexY + indexX;
}

const Color get(const ScreenBuffer *screen, int x, int y)
{
    Color color;
    color.r = 0;
    color.g = 0;
    color.b = 0;
    if (!screen)
        return color;

    int index = getIndex(x, y);
    if (index == -1)
        return color;

    return screen->buffer[index];
}

const float getDepthBuffer(const ScreenBuffer *screen, int x, int y)
{
    if (!screen)
        return INFINITY;

    int index = getIndex(x, y);
    if (index == -1)
        return INFINITY;

    return screen->depthBuffer[index];
}

int set(ScreenBuffer *screen, int x, int y, Color color)
{
    if (!screen)
        return 1;

    int index = getIndex(x, y);
    if (index == -1)
        return 1;

    screen->buffer[index] = color;
    return 0;
}

int setDepthBuffer(ScreenBuffer *screen, int x, int y, float depth)
{
    if (!screen)
        return 1;

    int index = getIndex(x, y);
    if (index == -1)
        return 1;

    screen->depthBuffer[index] = depth;
    return 0;
}

char *displayScreenBuffer(const ScreenBuffer *screen)
{
    // pre compute the size
    if (!screen)
        return NULL;

    char *output = malloc(1);
    if (!output)
        return NULL;
    output[0] = '\0';

    for (int y = -Y_OFFSET; y < HEIGHT - Y_OFFSET; y++)
    {
        for (int x = -X_OFFSET; x < WIDTH - X_OFFSET; x++)
        {
            Color color = get(screen, x, y);
            char *character = displayColor(color);
            size_t oldLength = strlen(output);
            size_t addLength = strlen(character);
            char *temporary = realloc(output, oldLength + addLength + 1);
            if (!temporary)
            {
                free(output);
                return NULL;
            }
            output = temporary;
            memcpy(output + oldLength, character, addLength + 1);
        }

        size_t oldLength = strlen(output);
        char *temporary = realloc(output, oldLength + 2);
        if (!temporary)
        {
            free(output);
            return NULL;
        }
        output = temporary;
        output[oldLength] = '\n';
        output[oldLength + 1] = '\0';
    }

    return output;
}

int drawTriangle(ScreenBuffer *screen, Vector3 *a, Vector3 *b, Vector3 *c, Color color)
{
    if (!screen)
        return 1;

    int maxX = (int)ceilf(max(a->x, max(b->x, c->x)));
    int minX = (int)floorf(min(a->x, min(b->x, c->x)));
    int maxY = (int)ceilf(max(a->y, max(b->y, c->y)));
    int minY = (int)floorf(min(a->y, min(b->y, c->y)));
    float near = 0.1;
    float far = 100.0;

    Vector2 a2 = {a->x, a->y};
    Vector2 b2 = {b->x, b->y};
    Vector2 c2 = {c->x, c->y};

    for (int y = minY; y < maxY; y++)
    {
        for (int x = minX; x < maxX; x++)
        {
            Vector2 p = {x, y};
            float u, v, w;
            float *weights = calculateBarycentricCoordinates(&a2, &b2, &c2, &p);

            if (!weights)
                continue;
            u = weights[0];
            v = weights[1];
            w = weights[2];

            if ((u >= 0.0) && (v >= 0.0) && (w >= 0.0))
            {
                float depth = normalizeDepth(u * a->z + v * b->z + w * c->z, near, far);

                if (depth < getDepthBuffer(screen, x, y))
                {
                    set(screen, x, y, color);
                    setDepthBuffer(screen, x, y, depth);
                }
            }
        }
    }

    return 0;
}

float normalizeDepth(const float z, const float near, const float far)
{
    return (z - near) / (far - near);
}

float *calculateBarycentricCoordinates(Vector2 *a, Vector2 *b, Vector2 *c, Vector2 *p)
{
    float denominator = (b->y - c->y) * (a->x - c->x) + (c->x - b->x) * (a->y - c->y);
    float u = ((b->y - c->y) * (p->x - c->x) + (c->x - b->x) * (p->y - c->y)) / denominator;
    float v = ((c->y - a->y) * (p->x - c->x) + (a->x - c->x) * (p->y - c->y)) / denominator;
    float w = 1.0 - u - v;
    float *weights = malloc(3 * sizeof(float));
    if (!weights)
        return NULL;
    weights[0] = u;
    weights[1] = v;
    weights[2] = w;
    return weights;
}

Vector3 projectCoordinate(const Vector3 *p, const float focalLength)
{
    float denominator = focalLength + p->z;
    if (denominator == 0.0)
        denominator = 0.00001;
    Vector3 projected = {
        (focalLength * p->x) / denominator,
        (focalLength * p->y) / denominator,
        p->z};
    return projected;
}

int clearScreenBuffer(ScreenBuffer *screen)
{
    if (!screen || !screen->buffer || !screen->depthBuffer)
        return 1;

    memset(screen->buffer, 0, BUFFER_SIZE * sizeof(Color));
    memset(screen->depthBuffer, 0, BUFFER_SIZE * sizeof(float));
    return 0;
}

int drawModel(ScreenBuffer *screen, const Model *model, const float focalLength)
{
    if (!screen || !model)
        return 1;

    for (size_t i = 0; i < model->faceCount; i++)
    {
        size_t faceIndex1 = model->faces[i][0];
        size_t faceIndex2 = model->faces[i][1];
        size_t faceIndex3 = model->faces[i][2];

        Vector3 *vertices = model->vertices;
        Vector3 vertex1 = projectCoordinate(&vertices[faceIndex1], focalLength);
        Vector3 vertex2 = projectCoordinate(&vertices[faceIndex2], focalLength);
        Vector3 vertex3 = projectCoordinate(&vertices[faceIndex3], focalLength);
        Color color = {randColor(), randColor(), randColor()};

        drawTriangle(screen, &vertex1, &vertex2, &vertex3, color);
    }
    return 0;
}

void freeScreenBuffer(ScreenBuffer *screen)
{
    if (screen)
    {
        free(screen->buffer);
        free(screen->depthBuffer);
        free(screen);
    }
}

int rotateModel(Model *model, const Axis rotationAxis, const float theta)
{
    if (!model)
        return 1;

    const float sinTheta = sinf(theta);
    const float cosTheta = cosf(theta);

    for (size_t i = 0; i < model->vertexCount; i++)
    {
        Vector3 *vertex = &model->vertices[i];
        switch (rotationAxis)
        {
        case X:
            vertex->y = vertex->y * cosTheta - vertex->z * sinTheta;
            vertex->z = vertex->y * sinTheta + vertex->z * cosTheta;
            break;
        case Y:
            vertex->x = vertex->x * cosTheta + vertex->z * sinTheta;
            vertex->z = -vertex->x * sinTheta + vertex->z * cosTheta;
            break;
        case Z:
            vertex->x = vertex->x * cosTheta - vertex->y * sinTheta;
            vertex->y = vertex->x * sinTheta + vertex->y * cosTheta;
            break;

        default:
            return 1;
        }
    }

    return 0;
}
