#include <stdio.h>
#include "raylib.h"

#include "screen.h"
#include "model.h"
#include "types.h"

int main()
{
    const int width = 1700;
    const int height = 960;
    const float mouseSensitivity = 3.0;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float scale = 1.0f;
    Vector2 lastMousePos = GetMousePosition();

    InitWindow(width, height, "Rasterizer-C");
    SetTargetFPS(60);

    ScreenBuffer *screen = initScreenBuffer(width, height);
    Model3D *model = initModel();
    loadModelFromFile(model, "../../objects/bird.obj", "../../textures/bird.jpg");

    Image image = {
        .data = screen->buffer,
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1};
    Texture2D texture = LoadTextureFromImage(image);

    while (!WindowShouldClose())
    {
        scale += GetMouseWheelMove() / 10.0f;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 currentMousePos = GetMousePosition();
            Vector2 delta = {
                currentMousePos.x - lastMousePos.x,
                currentMousePos.y - lastMousePos.y};

            pitch = delta.x / width * mouseSensitivity;
            yaw = delta.y / height * mouseSensitivity;

            rotateModel(model, Y, pitch);
            rotateModel(model, X, -yaw);

            lastMousePos = currentMousePos;
        }
        else
        {
            lastMousePos = GetMousePosition();
        }

        clearScreenBuffer(screen);
        drawModel(screen, model, scale);

        UpdateTexture(texture, screen->buffer);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    UnloadTexture(texture);
    freeModel(model);
    freeScreenBuffer(screen);
    CloseWindow();
    return 0;
}

// TODO better error handling