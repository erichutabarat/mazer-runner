#include "MazeGenerator.h"

void MazeGenerator::Generate(std::vector<Wall *> &walls)
{
    // 1 = Wall, 0 = Empty Path
    // You can make this as large as you want!
    const int MAP_WIDTH = 10;
    const int MAP_HEIGHT = 10;

    int map[MAP_WIDTH][MAP_HEIGHT] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    float wallSize = 2.0f;   // Each wall block is 2x2 units
    float wallHeight = 3.0f; // How tall the maze is

    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int z = 0; z < MAP_HEIGHT; z++)
        {
            if (map[x][z] == 1)
            {
                // Calculate position based on grid index
                // We center the maze by subtracting half the total width
                glm::vec3 pos(
                    (x - MAP_WIDTH / 2.0f) * wallSize,
                    wallHeight / 2.0f - 0.5f, // Center height offset
                    (z - MAP_HEIGHT / 2.0f) * wallSize);

                walls.push_back(new Wall(pos, glm::vec3(wallSize, wallHeight, wallSize)));
            }
        }
    }
}