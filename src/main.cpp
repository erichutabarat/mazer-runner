#include "Window.h"

int main()
{
    Window gameWindow(800, 600, "Maze Runner");

    while (!gameWindow.shouldClose())
    {
        // 1. Render
        gameWindow.clear(0.1f, 0.1f, 0.1f, 1.0f);

        // TODO: Draw your maze here!

        // 2. Swap buffers and Poll events
        gameWindow.update();
    }

    return 0;
}