#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Window.h"
#include "Camera.h"
#include "Floor.h"
#include "Wall.h"
#include "Item.h"
#include "ItemBox.h"
#include "HUD.h"

// --- GLOBALS ---
std::vector<Item> inventory;
std::vector<Item> worldItems;

// Global camera initialization
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = 400, lastY = 300;
bool firstMouse = true;

// --- CALLBACKS ---
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
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

int main()
{
    // 1. Initialize Window and HUD
    Window gameWindow(800, 600, "Maze Runner");

    // HUD class handles all ImGui Init internaly
    HUD hud(gameWindow.getNativeWindow());

    // 2. Input Configuration
    glfwSetInputMode(gameWindow.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(gameWindow.getNativeWindow(), mouse_callback);

    glEnable(GL_DEPTH_TEST);

    // 3. Initialize Game Objects
    Floor floor;
    ItemBox itemRenderer;

    std::vector<Wall *> gameWalls;
    // Jumpable obstacle
    gameWalls.push_back(new Wall(glm::vec3(0.0f, -0.125f, -5.0f), glm::vec3(2.0f, 0.75f, 1.0f)));
    // Tall boundary wall
    gameWalls.push_back(new Wall(glm::vec3(-5.0f, 0.5f, -2.0f), glm::vec3(1.0f, 2.0f, 10.0f)));

    // 4. Populate World Items
    worldItems.push_back(Item("Golden Key", ItemType::KEY, glm::vec3(2.0f, 0.0f, -3.0f), glm::vec3(1.0f, 0.8f, 0.0f)));
    worldItems.push_back(Item("Medkit", ItemType::HEALTH, glm::vec3(-2.0f, 0.0f, -4.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // --- MAIN LOOP ---
    while (!gameWindow.shouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // ONE function handles everything the player DOES
        camera.processKeyboard(gameWindow.getNativeWindow(), deltaTime, gameWalls, worldItems, inventory);

        // Rendering prep
        gameWindow.clear(0.4f, 0.7f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Draw everything
        floor.render(view, proj);
        for (auto w : gameWalls)
            w->render(view, proj);
        for (auto &item : worldItems)
        {
            if (!item.isPickedUp)
            {
                item.rotation += deltaTime * 60.0f;
                itemRenderer.render(item.position, item.rotation, item.color, view, proj);
            }
        }

        // Draw UI
        hud.beginFrame();
        hud.render(camera.m_Position, inventory, worldItems);
        hud.endFrame();

        gameWindow.update();
    }

    // Cleanup
    for (auto w : gameWalls)
        delete w;

    // HUD Destructor handles ImGui Shutdown
    return 0;
}