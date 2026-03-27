#include <GL/glew.h>
#include "Window.h"
#include "Floor.h"
#include "Camera.h"

int main()
{
    Window gameWindow(800, 600, "Maze Runner");
    Floor floor;

    // Initial position: x=0, y=2 (above ground), z=10 (back)
    Camera camera(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!gameWindow.shouldClose())
    {
        // 1. Calculate DeltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 2. Process Input
        camera.processKeyboard(gameWindow.getNativeWindow(), deltaTime);

        // 3. Render
        gameWindow.clear(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        floor.render(view, proj);

        gameWindow.update();
    }
    return 0;
}