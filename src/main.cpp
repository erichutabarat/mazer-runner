#include <GL/glew.h>
#include "Window.h"
#include "Camera.h"
#include "Floor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    Floor floor;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!gameWindow.shouldClose())
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // 1. New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // 2. Create a transparent "Debug Overlay" at the top left
        ImGui::SetNextWindowPos(ImVec2(10, 10)); // Top-left corner
        ImGui::Begin("Debug", nullptr,
                     ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoBackground);

        // Show the coordinates from your camera object
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Player Position:");
        ImGui::Text("X: %.2f", camera.m_Position.x);
        ImGui::Text("Y: %.2f", camera.m_Position.y);
        ImGui::Text("Z: %.2f", camera.m_Position.z);

        ImGui::End();
        camera.processKeyboard(gameWindow.getNativeWindow(), deltaTime);

        gameWindow.clear(0.4f, 0.7f, 1.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        floor.render(view, proj);
        // 3. Render ImGui on top of everything else
        // 4. Render ImGui UI on top of the game
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        gameWindow.update();
    }
    return 0;
}