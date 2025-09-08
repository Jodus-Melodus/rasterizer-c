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
    srand((unsigned int)time(NULL));
    SetConsoleSize();
    EnableANSI();

    ScreenBuffer *screen = initScreenBuffer();
    Model *model = initModel();
    loadModelFromFile(model, "../../objects/cone.obj");
    const float focalLength = 50.0;

    int running = 1;
    while (running)
    {
        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 27)
                running = 0;
        }

        clearScreenBuffer(screen);
        drawModel(screen, model, focalLength);
        system("cls");
        char *display = displayScreenBuffer(screen);
        printf("%s", display);
        Sleep(100);
    }

    return 0;
}