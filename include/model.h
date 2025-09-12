#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "stb_image.h"

typedef struct
{
    Vector3 *vertices;
    size_t vertexCount;
    size_t (*faces)[6];
    size_t faceCount;
    Color *texture;
    size_t textureWidth;
    size_t textureHeight;
    Vector2 *textureCoordinates;
    size_t textureCoordinateCount;
} Model3D;

Model3D *initModel();
int loadModelFromFile(Model3D *model, const char *modelPath, const char *texturePath);
void freeModel(Model3D *model);

#endif