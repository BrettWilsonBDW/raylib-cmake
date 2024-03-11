#include "raylib.h"
#include <cmath> // for sin and cos

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// used to get web width and height in browser
#if defined(PLATFORM_WEB)
EM_JS(int, getBrowserWidth, (), {
    return window.innerWidth;
});

EM_JS(int, getBrowserHeight, (), {
    return window.innerHeight;
});
#endif

// class used to set up game loop
class GameLoop
{
public:
    GameLoop();
    ~GameLoop();
    void Run();

private:
    void Draw();
    static void MainLoopHelper(void *userData); // needed for emscripten
    int screenWidth{};
    int screenHeight{};
};

GameLoop::GameLoop()
{
    screenWidth = 1920 / 2;
    screenHeight = 1080 / 2;

#if defined(PLATFORM_WEB)
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
#else
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE); // set custom flags for desktop
    SetTargetFPS(60);                                                            // do not set fps when used in browser for better web performance
#endif

    InitWindow(screenWidth, screenHeight, "Game name");
    Run();
}

GameLoop::~GameLoop()
{
    CloseWindow();
}

void GameLoop::Draw()
{
    // set proper width and height in browser window
#if defined(PLATFORM_WEB)
    int padding = 30; // set padding to avoid scrollbar and browser edge overlap
    SetWindowSize(getBrowserWidth() - padding, getBrowserHeight() - padding);
#endif

    BeginDrawing();
    ClearBackground(DARKGRAY);

    // rainbow triangle 
    {
        static unsigned char rainbowNum{0};

        DrawTriangle(
            Vector2{static_cast<float>(screenWidth / 2), static_cast<float>(screenHeight / 2 - 100)},
            Vector2{static_cast<float>(screenWidth / 2 - 100), static_cast<float>(screenHeight / 2 + 100)},
            Vector2{static_cast<float>(screenWidth / 2 + 100), static_cast<float>(screenHeight / 2 + 100)},
            Color{
                static_cast<unsigned char>((1 + sin(0.3 * rainbowNum)) * 127.5),
                static_cast<unsigned char>((1 + sin(0.3 * rainbowNum + 2)) * 127.5),
                static_cast<unsigned char>((1 + sin(0.3 * rainbowNum + 4)) * 127.5),
                255});

        rainbowNum++;
        if (rainbowNum > 256)
        {
            rainbowNum = 0;
        }
    }

    EndDrawing();
}

// method used to call all game logic
void GameLoop::MainLoopHelper(void *userData)
{
    GameLoop *gameLoop = static_cast<GameLoop *>(userData);

    gameLoop->Draw(); // add more game logic methods here eg: Update();
}

// used to run game loop logic
void GameLoop::Run()
{
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(MainLoopHelper, this, 0, 1);
#else
    while (!WindowShouldClose())
    {
        MainLoopHelper(this);
    }
#endif
}

int main()
{
    GameLoop gameLoop; // create game loop object

    return 0;
}