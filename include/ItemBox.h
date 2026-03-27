#ifndef ITEMBOX_H
#define ITEMBOX_H

#include "Objects.h"
#include <glm/glm.hpp>

class ItemBox : public Objects
{
public:
    ItemBox(); // Basic setup for a small cube
    void render(glm::vec3 position, float rotation, glm::vec3 color, glm::mat4 view, glm::mat4 projection);
};

#endif