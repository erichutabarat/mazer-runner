#include "HUD.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

HUD::HUD(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

HUD::~HUD()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void HUD::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void HUD::render(const glm::vec3 &playerPos, std::vector<Item> &inventory, std::vector<Item> &worldItems)
{
    showDebugOverlay(playerPos);
    showInventory(inventory, worldItems);
}

void HUD::showDebugOverlay(const glm::vec3 &pos)
{
    ImGui::SetNextWindowPos(ImVec2(10, 10));
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Pos: %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);
    ImGui::End();
}

void HUD::showInventory(std::vector<Item> &inventory, std::vector<Item> &worldItems)
{
    ImGui::Begin("Inventory");
    if (inventory.empty())
    {
        ImGui::Text("Empty.");
    }
    else
    {
        for (int i = 0; i < inventory.size(); i++)
        {
            ImGui::BulletText("%s", inventory[i].name.c_str());
            ImGui::SameLine();
            if (ImGui::SmallButton(("Drop##" + std::to_string(i)).c_str()))
            {
                // Find item in world list to reset its position
                for (auto &wi : worldItems)
                {
                    if (wi.name == inventory[i].name && wi.isPickedUp)
                    {
                        wi.isPickedUp = false;
                        wi.position = glm::vec3(0, 0, 0); // Logic handled in main usually
                        break;
                    }
                }
                inventory.erase(inventory.begin() + i);
            }
        }
    }
    ImGui::End();
}

void HUD::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}