#include "raylib.h"
#include "Debug.hpp"

int main()
{
    // rgb and transparency
    Color black = Color{0, 0, 0, 255};

    int windowWidth{1920 / 2};
    int windowHeight{1080 / 2};

    InitWindow(windowWidth, windowHeight, "Game name");
    // resize window
    windowWidth = GetScreenWidth();
    windowHeight = GetScreenHeight();
    SetWindowSize(windowWidth, windowHeight);
    // SetWindowPosition(1920, 10);

    SetExitKey(KEY_NULL);

    //init the debug class
    Debug debug;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(black);

        // debug  menu
        if (debug.ToggleState())
        {
            int fps = GetFPS();

            // call drawItems once and passItems before as many times as needed

            debug.PassItems("Debug Menu:\n");
            debug.PassItems("FPS: %d\n", fps);
            debug.DrawItems(3, 3, 16);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
