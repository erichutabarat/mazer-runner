#include <GL/glew.h>
#include "Window.h"
#include "Camera.h"
#include "Floor.h"

// Global camera so the callback can see it
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = 400, lastY = 300;
bool firstMouse = true;

// This function is called by GLFW every time the mouse moves
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

int main()
{
    Window gameWindow(800, 600, "Maze Runner");

    // 1. Tell GLFW to capture our mouse
    glfwSetInputMode(gameWindow.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 2. Set the callback function
    glfwSetCursorPosCallback(gameWindow.getNativeWindow(), mouse_callback);

    Floor floor;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!gameWindow.shouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processKeyboard(gameWindow.getNativeWindow(), deltaTime);

        gameWindow.clear(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        floor.render(view, proj);

        gameWindow.update();
    }
    return 0;
}