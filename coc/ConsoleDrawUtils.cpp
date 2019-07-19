#include "stdafx.h"
#include "ConsoleDrawUtils.h"
#include <iostream>

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void DrawPosToConsole(short x, short y, const char drawChar, int color)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(hConsole, pos);
    SetConsoleTextAttribute(hConsole, color);
    std::cout << drawChar;
}

void DrawBoxToConsole(int minX, int maxX, int minY, int maxY)
{
    COORD pos;
	static const char boxChar[2] = { '.' };
    for (int x = minX; x <= maxX; ++x)
    {
        DrawPosToConsole(x, minY, boxChar[0]);
        DrawPosToConsole(x, maxY, boxChar[0]);
    }
    for (int y = minY; y <= maxY; ++y)
    {
        DrawPosToConsole(minX, y, boxChar[0]);
        DrawPosToConsole(maxX, y, boxChar[0]);
        pos = { static_cast<short>(minX), static_cast<short>(y) };
    }
}