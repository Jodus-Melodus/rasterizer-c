#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

#include "screen.h"
#include "model.h"

void EnableANSI()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void SetConsoleSize()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize;
    bufferSize.X = WIDTH;
    bufferSize.Y = HEIGHT;
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SMALL_RECT windowSize = {0, 0, WIDTH - 1, HEIGHT - 1};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

int main()
{
    SetConsoleSize();
    EnableANSI();

    clock_t lastFrame, now;
    float deltaTime;
    lastFrame = clock();
    float rotationSpeed = 1.0;

    ScreenBuffer *screen = initScreenBuffer();
    Model *model = initModel();
    loadModelFromFile(model, "../../objects/cube.obj", "../../textures/test.png");
    const float focalLength = 100.0;

    int running = 1;
    while (running)
    {
        now = clock();
        deltaTime = (float)(now - lastFrame) / CLOCKS_PER_SEC / 120;

        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 27)
                running = 0;
        }

        rotateModel(model, Y, deltaTime * rotationSpeed);
        clearScreenBuffer(screen);
        drawModel(screen, model, focalLength);
        char *display = displayScreenBuffer(screen);
        printf("\x1b[2J\x1b[H");
        printf("%s", display);
        Sleep(100);
    }

    return 0;
}

// TODO metadata in separate window
// TODO better error handeling