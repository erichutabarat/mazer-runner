#ifndef WALL_H
#define WALL_H

#include "Objects.h"
#include <glm/glm.hpp>

class Wall : public Objects
{
public:
    // position = center of wall, size = width/height/depth
    Wall(glm::vec3 position, glm::vec3 size);

    void render(glm::mat4 view, glm::mat4 projection);

    // The "Magic" function: Returns true if a point is inside this wall
    bool isColliding(glm::vec3 playerPos, float playerRadius);

private:
    glm::vec3 m_Position;
    glm::vec3 m_Size;
    unsigned int m_ModelLoc;
};

#endif