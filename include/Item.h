#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <glm/glm.hpp>

enum class ItemType
{
    KEY,
    HEALTH,
    COIN,
    TREASURE
};

struct Item
{
    std::string name;
    ItemType type;
    glm::vec3 position;
    glm::vec3 color; // <--- ADD THIS
    bool isPickedUp;
    float rotation;

    // ADD THIS CONSTRUCTOR: It makes push_back work perfectly
    Item(std::string n, ItemType t, glm::vec3 pos, glm::vec3 col)
        : name(n), type(t), position(pos), color(col), isPickedUp(false), rotation(0.0f) {}
};

#endif