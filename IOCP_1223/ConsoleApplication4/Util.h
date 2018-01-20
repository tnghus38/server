#pragma once

#include <windows.h>

enum class eColor {
    Black, Blue, Green, Cyan, Red, Magenta, Brown,
    LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta,
    Yellow, White,
    None,
};

inline void ClearConsoleToColors(int ForgC, int BackC) {
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, wColor);
    return;
}

inline void gotoxy(short x, short y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

inline void SetCursor(bool bVisible) {
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = bVisible; // true ����, false �Ⱥ���
    ConsoleCursor.dwSize = 1; // Ŀ��������
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}