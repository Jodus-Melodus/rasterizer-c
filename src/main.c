#include <stdio.h>
#include <Windows.h>

#include "screen.h"
#include "model.h"
#include "types.h"

int main()
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD events;
    INPUT_RECORD inputRecord;
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    const int width = 208;
    const int height = 50;
    const float focalLength = 100.0;
    const float mouseSensitivity = 3.0;
    int running = 1;
    Vector2 mousePosition = {0};
    Vector2 deltaMouse = {0};
    float pitch, yaw;

    ScreenBuffer *screen = initScreenBuffer(width, height);
    Model *model = initModel();
    loadModelFromFile(model, "../../objects/torus.obj", "../../textures/test.png");

    while (running)
    {
        if (PeekConsoleInput(hInput, &inputRecord, 1, &events) && events > 0)
        {
            ReadConsoleInput(hInput, &inputRecord, 1, &events);
            if (inputRecord.EventType == MOUSE_EVENT)
            {
                MOUSE_EVENT_RECORD mouse = inputRecord.Event.MouseEvent;
                Vector2 newMousePosition = {
                    mouse.dwMousePosition.X,
                    mouse.dwMousePosition.Y};

                if (mouse.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                {
                    deltaMouse.x = newMousePosition.x - mousePosition.x;
                    deltaMouse.y = newMousePosition.y - mousePosition.y;
                }
                else
                {
                    deltaMouse.x = 0;
                    deltaMouse.y = 0;
                }
                mousePosition = newMousePosition;
            }
        }

        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 27)
                running = 0;
        }

        pitch = deltaMouse.x / (float)width * mouseSensitivity;
        yaw = deltaMouse.y / (float)height * mouseSensitivity;
        rotateModel(model, Y, -pitch);
        rotateModel(model, X, -yaw);

        clearScreenBuffer(screen);
        drawModel(screen, model, focalLength);
        char *display = displayScreenBuffer(screen);
        printf("\x1b[H");
        printf("%s", display);
        Sleep(10);
    }

    return 0;
}

// TODO metadata in separate window
// TODO better error handeling