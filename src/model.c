#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "model.h"

int initModel(Model *model)
{
    model->vertices = NULL;
    model->faces = NULL;
    model->vertexCount = 0;
    model->faceCount = 0;
    return 0;
}

int loadModelFromFile(Model *model, const char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return -1;
    }

    char lineBuffer[256];
    while (fgets(lineBuffer, sizeof(lineBuffer), file))
    {
        char lineType[2];
        float x, y, z;

        if (lineBuffer[0] == 'v' && lineBuffer[1] == ' ')
        {
            if (sscanf(lineBuffer, "v %f %f %f", &x, &y, &z) != 3)
            {
                fprintf(stderr, "Invalid vertext line: %s\n", lineBuffer);
                continue;
                        }
            Vector3 vec = {x, y, z};
            Vector3 *newVertexPointer = realloc(model->vertices, (model->vertexCount + 1) * sizeof(Vector3));
            if (newVertexPointer == NULL)
            {
                free(model->vertices);
                fclose(file);
                return -1;
            }
            model->vertices = newVertexPointer;
            model->vertices[model->vertexCount] = vec;
            model->vertexCount++;
        }
        else if (lineBuffer[0] == 'f' && lineBuffer[1] == ' ')
        {
            char *pointer = lineBuffer + 2;
            int *faceIndices = NULL;
            size_t faceIndicesCount = 0;

            char *token = strtok(pointer, " \t\n");
            while (token)
            {
                int index;
                if (sscanf(token, "%d", &index) == 1)
                {
                    index--;

                    int *temporary = realloc(faceIndices, (faceIndicesCount + 1) * sizeof(int));
                    if (temporary == NULL)
                    {
                        free(faceIndices);
                        fclose(file);
                        return -1;
                    }
                    faceIndices = temporary;
                    faceIndices[faceIndicesCount++] = index;
                }
                token = strtok(NULL, " \t\n");
            }

            for (size_t i = 1; i < faceIndicesCount - 1; i++)
            {
                size_t (*newFacePointer)[3] =
                    realloc(model->faces, (model->faceCount + 1) * sizeof(size_t[3]));
                if (newFacePointer == NULL)
                {
                    free(faceIndices);
                    free(model->faces);
                    fclose(file);
                    return -1;
                }

                model->faces = newFacePointer;

                model->faces[model->faceCount][0] = faceIndices[0];
                model->faces[model->faceCount][1] = faceIndices[i];
                model->faces[model->faceCount][2] = faceIndices[i + 1];
                model->faceCount++;
            }

            free(faceIndices);
        }
    }

    fclose(file);
    return 0;
}

int freeModel(Model *model)
{
    free(model->faces);
    free(model->vertices);
    model->faces = NULL;
    model->vertices = NULL;
    model->faceCount = 0;
    model->vertexCount = 0;
    return 0;
}
