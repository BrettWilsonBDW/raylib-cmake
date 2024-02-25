#pragma once

#include <iostream>
#include "raylib.h"
#include <cstdarg>
#include <cstring>

class Debug
{
public:
    // Indicates whether the debug menu is currently active or not from toggleDebug().
    bool currentState{false};

    Debug();
    bool ToggleState();
    void ClearLines(int linesToClear);
    void PassItems(const char *format = "Debug Menu:\n", ...);
    void DrawItems(int x, int y, int fontSize, int useFont = 1, int spacing = 2, Color color = WHITE);
    void SetFont(Font setfont)
    {
        font = setfont;
    }

private:
    // toggle debug
    int toggleDebugKey{};
    bool toggleDebugMenu{false};
    char buffer[256]; // Assuming a maximum buffer size of 256 characters
    Font font = LoadFont("assets/fonts/Roboto.ttf");
    // Font font;
};