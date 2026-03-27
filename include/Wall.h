#ifndef WALL_H
#define WALL_H

#include "Objects.h"
#include <glm/glm.hpp>

class Wall : public Objects
{
public:
    Wall(glm::vec3 position, glm::vec3 size);
    void render(glm::mat4 view, glm::mat4 projection);
    bool isColliding(glm::vec3 playerPos, float playerRadius);

    // ADD THESE TWO LINES HERE:
    glm::vec3 getPosition() const { return m_Position; }
    glm::vec3 getSize() const { return m_Size; }

private:
    glm::vec3 m_Position;
    glm::vec3 m_Size;
    unsigned int m_ModelLoc;
};

#endif