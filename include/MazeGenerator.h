#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <vector>
#include "Wall.h"
#include <glm/glm.hpp>

class MazeGenerator
{
public:
    // This function populates your gameWalls vector based on a grid layout
    static void Generate(std::vector<Wall *> &walls);
};

#endif