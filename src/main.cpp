#include <GL/glew.h>
#include "Window.h"
#include "Camera.h"
#include "Floor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <Wall.h>

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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(gameWindow.getNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // 1. Tell GLFW to capture our mouse
    glfwSetInputMode(gameWindow.getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 2. Set the callback function
    glfwSetCursorPosCallback(gameWindow.getNativeWindow(), mouse_callback);

    // wall
    // Outside the loop
    std::vector<Wall *> gameWalls;
    // Height decreased from 2.0 to 0.75, and Y-position lowered to stay on floor
    gameWalls.push_back(new Wall(glm::vec3(0.0f, -0.125f, -5.0f), glm::vec3(2.0f, 0.75f, 1.0f)));

    Floor floor;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!gameWindow.shouldClose())
    {
        // 1. Time & Input FIRST
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Move the player BEFORE calculating the matrices
        camera.processKeyboard(gameWindow.getNativeWindow(), deltaTime, gameWalls);

        // 2. Math SECOND
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // 3. ImGui Logic
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Player Position:");
        ImGui::Text("X: %.2f | Y: %.2f | Z: %.2f", camera.m_Position.x, camera.m_Position.y, camera.m_Position.z);
        ImGui::End();

        // 4. Rendering THIRD
        gameWindow.clear(0.4f, 0.7f, 1.0f, 1.0f); // Nice sky blue!
        glClear(GL_DEPTH_BUFFER_BIT);

        floor.render(view, proj);
        for (auto w : gameWalls)
            w->render(view, proj);

        // 5. UI LAST (So it draws on top of the 3D world)
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        gameWindow.update();
    }
    return 0;
}