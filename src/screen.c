#include "screen.h"
#include "utils.h"

ScreenBuffer *initScreenBuffer(const int width, const int height)
{
    ScreenBuffer *screen = malloc(sizeof(ScreenBuffer));
    if (!screen)
        return NULL;
    screen->buffer = malloc(width * height * sizeof(Color));
    screen->depthBuffer = malloc(width * height * sizeof(float));
    if (!screen->buffer || !screen->depthBuffer)
    {
        free(screen);
        return NULL;
    }
    memset(screen->buffer, 0, width * height * sizeof(Color));
    memset(screen->depthBuffer, 0, width * height * sizeof(float));
    screen->width = width;
    screen->height = height;

    return screen;
}

int getIndex(int x, int y, const int width, const int height)
{
    int indexX = x + (width / 2);
    int indexY = y + (height / 2);

    if (indexX < 0 || indexX >= width || indexY < 0 || indexY >= height)
        return -1;

    return width * indexY + indexX;
}

const Color get(const ScreenBuffer *screen, int x, int y)
{
    Color color = {0, 0, 0};
    if (!screen)
        return color;

    int index = getIndex(x, y, screen->width, screen->height);
    if (index == -1)
        return color;

    return screen->buffer[index];
}

const float getDepthBuffer(const ScreenBuffer *screen, int x, int y)
{
    if (!screen)
        return INFINITY;

    int index = getIndex(x, y, screen->width, screen->height);
    if (index == -1)
        return INFINITY;

    return screen->depthBuffer[index];
}

int set(ScreenBuffer *screen, int x, int y, Color color)
{
    if (!screen)
        return 1;

    int index = getIndex(x, y, screen->width, screen->height);
    if (index == -1)
        return 1;

    screen->buffer[index] = color;
    return 0;
}

int setDepthBuffer(ScreenBuffer *screen, int x, int y, float depth)
{
    if (!screen)
        return 1;

    int index = getIndex(x, y, screen->width, screen->height);
    if (index == -1)
        return 1;

    screen->depthBuffer[index] = depth;
    return 0;
}

int drawTriangle(ScreenBuffer *screen, Vector3 *a, Vector3 *b, Vector3 *c,
                 Vector2 textureCoordinate1, Vector2 textureCoordinate2, Vector2 textureCoordinate3,
                 Color *texture, const size_t textureWidth, const size_t textureHeight)
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

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            Vector2 p = {x, y};
            float u, v, w;
            if (!calculateBarycentricCoordinates(a2, b2, c2, p, &u, &v, &w))
                continue;

            if ((u >= 0.0) && (v >= 0.0) && (w >= 0.0))
            {
                float depth = normalizeDepth(u * a->z + v * b->z + w * c->z, near, far);
                int textureCoordinateX, textureCoordinateY;
                if (!interpolateTextureCoordinates(textureCoordinate1, textureCoordinate2,
                                                   textureCoordinate3, u, v, w, &textureCoordinateX, &textureCoordinateY,
                                                   textureWidth, textureHeight))
                    continue;
                Color color = texture[textureCoordinateY * textureWidth + textureCoordinateX];

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

inline int interpolateTextureCoordinates(Vector2 textureCoordinate1, Vector2 textureCoordinate2,
                                         Vector2 textureCoordinate3, float u, float v, float w, int *textureU, int *textureV,
                                         const size_t textureWidth, const size_t textureHeight)
{
    float textureX = u * textureCoordinate1.x + v * textureCoordinate2.x + w * textureCoordinate3.x;
    float textureY = u * textureCoordinate1.y + v * textureCoordinate2.y + w * textureCoordinate3.y;
    float scaledX = textureX * ((float)textureWidth - 1.0f);
    float scaledY = textureY * ((float)textureHeight - 1.0f);
    *textureU = (int)clamp(roundf(scaledX), 0.0f, (float)(textureWidth - 1));
    *textureV = (int)clamp(roundf(scaledY), 0.0f, (float)(textureHeight - 1));
    return 1;
}

inline int calculateBarycentricCoordinates(Vector2 a, Vector2 b, Vector2 c,
                                           Vector2 p, float *u, float *v, float *w)
{
    float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    if (denom == 0.0f)
        return 0;
    *u = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
    *v = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
    *w = 1.0f - *u - *v;
    return 1;
}

int clearScreenBuffer(ScreenBuffer *screen)
{
    if (!screen || !screen->buffer || !screen->depthBuffer)
        return 1;

    memset(screen->buffer, 0, screen->height * screen->width * sizeof(Color));

    for (size_t i = 0; i < screen->height * screen->width; ++i)
        screen->depthBuffer[i] = INFINITY;

    return 0;
}

Vector3 projectCoordinate(const Vector3 *p, RasterMatrix4 projectionMatrix)
{
    Vector4 v = {p->x, p->y, p->z, 1.0f};
    Vector4 projected = mat4_mul_vec4(projectionMatrix, v);

    if (fabsf(projected.w) < 1e-6f)
        projected.w = 1e-6f;
    projected.x /= projected.w;
    projected.y /= projected.w;
    projected.z /= projected.w;

    return (Vector3){projected.x, projected.y, projected.z};
}

int drawModel(ScreenBuffer *screen, const Model3D *model, const float focalLength)
{
    // FIXME dont map textures when there isn't a texture
    if (!screen || !model)
        return 1;

    float aspect = (float)screen->width / (float)screen->height;
    RasterMatrix4 projectionMatrix = perspective(PI / 3.0f, aspect, 0.1f, 100.0f);

    for (size_t i = 0; i < model->faceCount; i++)
    {
        size_t faceIndex1 = model->faces[i][0];
        size_t faceIndex2 = model->faces[i][1];
        size_t faceIndex3 = model->faces[i][2];
        size_t textureIndex1 = model->faces[i][3];
        size_t textureIndex2 = model->faces[i][4];
        size_t textureIndex3 = model->faces[i][5];

        Vector3 *vertices = model->vertices;
        Vector2 *textureCoordinates = model->textureCoordinates;
        Color *texture = model->texture;
        const size_t textureWidth = model->textureWidth;
        const size_t textureHeight = model->textureHeight;
        Vector3 vertex1 = projectCoordinate(&vertices[faceIndex1], projectionMatrix);
        Vector3 vertex2 = projectCoordinate(&vertices[faceIndex2], projectionMatrix);
        Vector3 vertex3 = projectCoordinate(&vertices[faceIndex3], projectionMatrix);
        Vector2 textureCoordinate1 = textureCoordinates[textureIndex1];
        Vector2 textureCoordinate2 = textureCoordinates[textureIndex2];
        Vector2 textureCoordinate3 = textureCoordinates[textureIndex3];

        drawTriangle(screen, &vertex1, &vertex2, &vertex3, textureCoordinate1,
                     textureCoordinate2, textureCoordinate3, texture, textureWidth,
                     textureHeight);
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

int rotateModel(Model3D *model, const Axis rotationAxis, const float theta)
{
    if (!model)
        return 1;

    const float sinTheta = sinf(theta);
    const float cosTheta = cosf(theta);

    for (size_t i = 0; i < model->vertexCount; i++)
    {
        Vector3 *vertex = &model->vertices[i];
        const float x = vertex->x;
        const float y = vertex->y;
        const float z = vertex->z;

        switch (rotationAxis)
        {
        case X:
            vertex->y = y * cosTheta - z * sinTheta;
            vertex->z = y * sinTheta + z * cosTheta;
            break;
        case Y:
            vertex->x = x * cosTheta + z * sinTheta;
            vertex->z = -x * sinTheta + z * cosTheta;
            break;
        case Z:
            vertex->x = x * cosTheta - y * sinTheta;
            vertex->y = x * sinTheta + y * cosTheta;
            break;

        default:
            return 1;
        }
    }

    return 0;
}
