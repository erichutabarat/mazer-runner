#ifndef HUD_H
#define HUD_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "imgui.h"
#include "Item.h"
#include <GLFW/glfw3.h>

class HUD
{
public:
    HUD(GLFWwindow *window);
    ~HUD();

    void beginFrame();
    void render(const glm::vec3 &playerPos, std::vector<Item> &inventory, std::vector<Item> &worldItems);
    void endFrame();

private:
    void showDebugOverlay(const glm::vec3 &pos);
    void showInventory(std::vector<Item> &inventory, std::vector<Item> &worldItems);
};

#endif