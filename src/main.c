#include <stdio.h>
#include "raylib.h"
// #include <Windows.h>

#include "screen.h"
#include "model.h"
#include "types.h"

int main()
{
    // HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    // DWORD events;
    // INPUT_RECORD inputRecord;
    // SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    const int width = 1280;
    const int height = 720;
    const float focalLength = 100.0;
    const float mouseSensitivity = 3.0;
    Point2D mousePosition = {0};
    Point2D deltaMouse = {0};
    float pitch, yaw;

    InitWindow(width, height, "Rasterizer-C");
    SetTargetFPS(60);

    ScreenBuffer *screen = initScreenBuffer(width, height);
    Model3D *model = initModel();
    loadModelFromFile(model, "../../objects/car.obj", "../../textures/car.png");

    Image image = {
        .data = screen->buffer,
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1};
    Texture2D texture = LoadTextureFromImage(image);

    while (!WindowShouldClose())
    {
        // if (PeekConsoleInput(hInput, &inputRecord, 1, &events) && events > 0)
        // {
        //     ReadConsoleInput(hInput, &inputRecord, 1, &events);
        //     if (inputRecord.EventType == MOUSE_EVENT)
        //     {
        //         MOUSE_EVENT_RECORD mouse = inputRecord.Event.MouseEvent;
        //         Point2D newMousePos = {
        //             mouse.dwMousePosition.X,
        //             mouse.dwMousePosition.Y};

        //         if (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
        //         {
        //             deltaMouse.x = newMousePos.x - mousePosition.x;
        //             deltaMouse.y = newMousePos.y - mousePosition.y;

        //             pitch = deltaMouse.x / (float)width * mouseSensitivity;
        //             yaw = deltaMouse.y / (float)height * mouseSensitivity;

        //             rotateModel(model, Y, -pitch);
        //             rotateModel(model, X, -yaw);
        //         }

        //         mousePosition = newMousePos;
        //     }
        // }

        clearScreenBuffer(screen);
        drawModel(screen, model, focalLength);

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