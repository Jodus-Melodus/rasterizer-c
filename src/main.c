#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
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
    ScreenBuffer screen;
    initScreenBuffer(&screen);

    int running = 1;
    while (running)
    {
        system("cls");

        if (_kbhit())
        {
            int ch = _getch();
            if (ch == 27)
            {
                running = 0;
            }
        }

        Vector2 a = {-10, -10};
        Vector2 b = {10, -10};
        Vector2 c = {0, 10};
        Color color = {255, 255, 255};
        drawTriangle(&screen, a, b, c, color);

        printf("%s", display(&screen));

        clearScreenBuffer(&screen);
        Sleep(500);
    }

    return 0;
}