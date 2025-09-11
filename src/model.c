#include "model.h"

Model3D *initModel()
{
    Model3D *model = malloc(sizeof(Model3D));
    model->vertices = NULL;
    model->faces = NULL;
    model->textureCoordinates = NULL;
    model->texture = NULL;

    model->vertexCount = 0;
    model->faceCount = 0;
    model->textureWidth = 0;
    model->textureHeight = 0;
    model->textureCoordinateCount = 0;
    return model;
}

int loadModelFromFile(Model3D *model, const char *modelPath, const char *texturePath)
{
    FILE *file = fopen(modelPath, "r");
    if (!file)
    {
        fprintf(stderr, "Failed to open file: %s\n", modelPath);
        return 1;
    }

    char lineBuffer[256];
    while (fgets(lineBuffer, sizeof(lineBuffer), file))
    {
        if (lineBuffer[0] == 'v' && lineBuffer[1] == ' ')
        {
            float x, y, z;
            if (sscanf(lineBuffer, "v %f %f %f", &x, &y, &z) != 3)
            {
                fprintf(stderr, "Invalid vertex line: %s\n", lineBuffer);
                continue;
            }
            Point3D vec = {x, y, z};
            Point3D *newVertexPointer = realloc(model->vertices, (model->vertexCount + 1) * sizeof(Point3D));
            if (newVertexPointer == NULL)
            {
                free(model->vertices);
                fclose(file);
                return 1;
            }
            model->vertices = newVertexPointer;
            model->vertices[model->vertexCount] = vec;
            model->vertexCount++;
        }
        if (lineBuffer[0] == 'v' && lineBuffer[1] == 't')
        {
            float u, v;
            if (sscanf(lineBuffer, "vt %f %f", &u, &v) != 2)
            {
                fprintf(stderr, "Invalid texture coordinate line: %s\n", lineBuffer);
                continue;
            }
            Point2D uv = {u, v};
            uv.y = 1.0f - uv.y;
            Point2D *newTexturePointer = realloc(model->textureCoordinates, (model->textureCoordinateCount + 1) * sizeof(Point2D));
            if (newTexturePointer == NULL)
            {
                free(model->textureCoordinates);
                fclose(file);
                return 1;
            }
            model->textureCoordinates = newTexturePointer;
            model->textureCoordinates[model->textureCoordinateCount] = uv;
            model->textureCoordinateCount++;
        }
        else if (lineBuffer[0] == 'f' && lineBuffer[1] == ' ')
        {
            char *pointer = lineBuffer + 2;
            int *faceIndices = NULL;
            int *textureIndices = NULL;
            size_t faceIndexCount = 0;
            size_t textureIndexCount = 0;

            char *token = strtok(pointer, " \t\n");
            while (token)
            {
                int faceIndex, textureIndex, normalIndex;
                if (sscanf(token, "%d/%d", &faceIndex, &textureIndex, &normalIndex) == 2)
                {
                    faceIndex--;
                    textureIndex--;

                    int *temporaryFaceIndices = realloc(faceIndices, (faceIndexCount + 1) * sizeof(int));
                    if (temporaryFaceIndices == NULL)
                    {
                        free(faceIndices);
                        fclose(file);
                        return 1;
                    }
                    faceIndices = temporaryFaceIndices;
                    faceIndices[faceIndexCount++] = faceIndex;

                    int *temporaryTextureIndices = realloc(textureIndices, (textureIndexCount + 1) * sizeof(int));
                    if (temporaryTextureIndices == NULL)
                    {
                        free(textureIndices);
                        fclose(file);
                        return 1;
                    }
                    textureIndices = temporaryTextureIndices;
                    textureIndices[textureIndexCount++] = textureIndex;
                }
                token = strtok(NULL, " \t\n");
            }

            if (faceIndexCount != textureIndexCount)
            {
                free(faceIndices);
                free(textureIndices);
                fclose(file);
                fprintf(stderr, "Invalid file: %s\n", lineBuffer);
                return 1;
            }

            if (faceIndexCount < 3)
            {
                free(faceIndices);
                free(textureIndices);
                fclose(file);
                fprintf(stderr, "Face has fewer than 3 vertices: %s\n", lineBuffer);
                return 1;
            }

            for (size_t i = 1; i < faceIndexCount - 1; i++)
            {
                size_t (*newFacePointer)[6] =
                    realloc(model->faces, (model->faceCount + 1) * sizeof(size_t[6]));
                if (newFacePointer == NULL)
                {
                    free(faceIndices);
                    free(textureIndices);
                    fclose(file);
                    return 1;
                }

                model->faces = newFacePointer;
                model->faces[model->faceCount][0] = faceIndices[0];
                model->faces[model->faceCount][1] = faceIndices[i];
                model->faces[model->faceCount][2] = faceIndices[i + 1];
                model->faces[model->faceCount][3] = textureIndices[0];
                model->faces[model->faceCount][4] = textureIndices[i];
                model->faces[model->faceCount][5] = textureIndices[i + 1];
                model->faceCount++;
            }

            free(faceIndices);
            free(textureIndices);
        }
    }
    fclose(file);

    int width, height, channels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &channels, 0);
    if (data == NULL)
    {
        fprintf(stderr, "Failed to open texture file\n");
        return 1;
    }

    PixelColor *texture = malloc(height * width * sizeof(PixelColor));
    if (texture == NULL)
    {
        stbi_image_free(data);
        return 1;
    }

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            int index = (y * width + x) * channels;
            unsigned char r = data[index + 0];
            unsigned char g = (channels > 1) ? data[index + 1] : 0;
            unsigned char b = (channels > 2) ? data[index + 2] : 0;
            unsigned char a = (channels > 3) ? data[index + 3] : 255;
            PixelColor color = {r, g, b, a};

            texture[y * width + x] = color;
        }
    }

    model->texture = texture;
    model->textureWidth = width;
    model->textureHeight = height;
    stbi_image_free(data);
    return 0;
}

void freeModel(Model3D *model)
{
    if (model)
    {
        free(model->faces);
        free(model->vertices);
        free(model->texture);
        free(model->textureCoordinates);
        free(model);
    }
}
