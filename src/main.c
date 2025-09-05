#include <stdio.h>
#include <stdlib.h>
#include "screen.h"
#include "model.h"

int main()
{
    struct Model model;
    initModel(&model);
    loadModelFromFile(&model, "/workspace/objects/cube.obj");

    printf("Loaded model from file with %ld vertices and %ld faces\n", model.vertexCount, model.faceCount);

    freeModel(&model);
    return 0;
}