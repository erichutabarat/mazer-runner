#include <GL/glew.h>
#include "Window.h"
#include "Camera.h"
#include "Floor.h"
#include "Wall.h"
#include "Item.h"
#include "ItemBox.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// --- GLOBALS ---
std::vector<Item> inventory;
std::vector<Item> worldItems;

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
    // 1. Initialize Window and Context
    Window gameWindow(800, 600, "Maze Runner");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(gameWindow.getNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glfwSetInputMode(gameWindow.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(gameWindow.getNativeWindow(), mouse_callback);

    glEnable(GL_DEPTH_TEST); // Vital for 3D!

    // 2. Initialize Game Objects
    Floor floor;
    ItemBox itemRenderer; // The 3D drawer for items

    std::vector<Wall *> gameWalls;
    // A jumpable obstacle
    gameWalls.push_back(new Wall(glm::vec3(0.0f, -0.125f, -5.0f), glm::vec3(2.0f, 0.75f, 1.0f)));
    // A tall boundary wall
    gameWalls.push_back(new Wall(glm::vec3(-5.0f, 0.5f, -2.0f), glm::vec3(1.0f, 2.0f, 10.0f)));

    // 3. Populate World Items
    // Format: { Name, Type, Position, Color, isPickedUp, Rotation }
    // Use the new constructor format: Item(Name, Type, Position, Color)
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

        // A. Input & Physics
        camera.processKeyboard(gameWindow.getNativeWindow(), deltaTime, gameWalls);

        // Interaction (Press E to pick up)
        if (glfwGetKey(gameWindow.getNativeWindow(), GLFW_KEY_E) == GLFW_PRESS)
        {
            for (auto &item : worldItems)
            {
                if (!item.isPickedUp)
                {
                    float dist = glm::distance(camera.m_Position, item.position);
                    if (dist < 1.8f) // Slightly larger reach for comfort
                    {
                        item.isPickedUp = true;
                        inventory.push_back(item);
                        std::cout << "Picked up: " << item.name << std::endl;
                    }
                }
            }
        }

        // B. Math
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // C. UI Frame Start
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Debug Overlay
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Position: %.1f, %.1f, %.1f", camera.m_Position.x, camera.m_Position.y, camera.m_Position.z);
        ImGui::End();

        // Inventory Window
        ImGui::Begin("Inventory");
        if (inventory.empty())
        {
            ImGui::Text("No items collected.");
        }
        else
        {
            for (int i = 0; i < inventory.size(); i++)
            {
                ImGui::BulletText("%s", inventory[i].name.c_str());
                ImGui::SameLine();
                if (ImGui::SmallButton(("Drop##" + std::to_string(i)).c_str()))
                {
                    // Find the item in worldItems to make it visible again at player feet
                    for (auto &wi : worldItems)
                    {
                        if (wi.name == inventory[i].name && wi.isPickedUp)
                        {
                            wi.isPickedUp = false;
                            wi.position = camera.m_Position - glm::vec3(0, 1.0f, 0); // Drop at feet
                            break;
                        }
                    }
                    inventory.erase(inventory.begin() + i);
                }
            }
        }
        ImGui::End();

        // D. Rendering
        gameWindow.clear(0.4f, 0.7f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        floor.render(view, proj);

        for (auto w : gameWalls)
            w->render(view, proj);

        for (auto &item : worldItems)
        {
            if (!item.isPickedUp)
            {
                item.rotation += deltaTime * 60.0f; // Spin animation
                itemRenderer.render(item.position, item.rotation, item.color, view, proj);
            }
        }

        // E. UI Finish
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        gameWindow.update();
    }

    // Cleanup
    for (auto w : gameWalls)
        delete w;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}