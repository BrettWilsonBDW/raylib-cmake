#include "Debug.hpp"

Debug::Debug()
{
    buffer[0] = '\0'; // Initialize the buffer as an empty string
    // font = LoadFont("assets/fonts/Roboto.ttf");
}

/**
 * Toggle the state of debugging.
 *
 * @return The new state of debugging.
 */
bool Debug::ToggleState()
{
    if (IsKeyPressed(96))
    {
        toggleDebugMenu = !toggleDebugMenu;
    }

    currentState = toggleDebugMenu;
    return toggleDebugMenu;
}

/**
 * Clears a specified number of lines in the terminal in order not to flood it by replacing the line with the updated var.
 *
 * @param linesToClear the number of lines to clear equivalent to number of /n and prints.
 *
 * @return void
 *
 * @throws None
 */
void Debug::ClearLines(int linesToClear)
{

    // magic code to not flood the terminal
    std::cout << "\033[" << linesToClear << "A\033[" << linesToClear << "K";
}

/**
 * Passes items to the debug output buffer to be drawn. Used with drawItems().
 *
 * Example: passItems("FPS: %d W: %d H: %d", fps, windowWidth, windowHeight);
 *
 * @param format A C-style format string.
 * @param ... Variable arguments based on the format string.
 *
 * @return void
 *
 * @throws None
 */
void Debug::PassItems(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Append new information to the existing buffer
    char tempBuffer[256];
    vsnprintf(tempBuffer, sizeof(tempBuffer), format, args);
    strncat(buffer, tempBuffer, sizeof(buffer) - strlen(buffer) - 1);

    va_end(args);
}

/**
 * Draws debug items on the screen at the specified coordinates with the specified font
 * size and color. You must call passItems() before calling this function.
 *
 * Example: drawItems(3, 3, 15, 2, WHITE);
 *
 * @param x The x-coordinate of the item.
 * @param y The y-coordinate of the item.
 * @param fontSize The size of the font used to draw the item.
 * @param useFont A flag indicating whether to use a custom font (1 for true, 0 for false: default).
 * @param spacing The spacing between the letters of the text. Default value is 2.
 * @param color The color used to draw the item. Default value is WHITE.
 *
 * @return void
 *
 * @throws None
 */
void Debug::DrawItems(int x, int y, int fontSize, int useFont, int spacing, Color color)
{
    if (useFont == 1)
    {
        DrawTextEx(font, buffer, Vector2{static_cast<float>(x), static_cast<float>(y)}, fontSize, spacing, color);
    }
    else
    {
        DrawText(buffer, x, y, fontSize, color);
    }

    // DrawText(buffer, x, y, fontSize, color);

    // Clear the buffer after drawing to avoid duplicating text
    buffer[0] = '\0';
}
