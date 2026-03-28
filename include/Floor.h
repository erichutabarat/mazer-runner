#ifndef FLOOR_H
#define FLOOR_H

#include "Objects.h"
#include <glm/glm.hpp>

class Floor : public Objects
{
public:
    Floor();

    // We add a special draw function that handles its own matrices
    void render(glm::mat4 view, glm::mat4 projection, unsigned int textureID);
};

#endif