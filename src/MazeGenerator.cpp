#include "MazeGenerator.h"

void MazeGenerator::Generate(std::vector<Wall *> &walls)
{
    // Make the grid much larger to accommodate a field + a maze
    const int MAP_WIDTH = 30;
    const int MAP_HEIGHT = 30;

    // Define the boundaries of your "Safe Field" in the center
    // For a 30x30 map, the center is 15. Let's make a 10x10 empty field.
    int fieldStart = 10;
    int fieldEnd = 20;

    float wallSize = 4.0f;
    float wallHeight = 15.0f; // Tall walls for a "Maze Runner" feel

    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int z = 0; z < MAP_HEIGHT; z++)
        {
            // 1. Check if we are in the center "Field" area
            bool isInsideField = (x > fieldStart && x < fieldEnd && z > fieldStart && z < fieldEnd);

            // 2. Check if we are the "Border" of the field (The giant surrounding walls)
            bool isFieldWall = ((x == fieldStart || x == fieldEnd) && (z >= fieldStart && z <= fieldEnd) ||
                                (z == fieldStart || z == fieldEnd) && (x >= fieldStart && x <= fieldEnd));

            // 3. Logic for placing walls
            bool shouldPlaceWall = false;

            if (isInsideField)
            {
                shouldPlaceWall = false; // Keep the center empty
            }
            else if (isFieldWall)
            {
                // Create an opening in the front so the player can actually enter the maze
                if (!(x == MAP_WIDTH / 2 && z == fieldEnd))
                {
                    shouldPlaceWall = true;
                }
            }
            else
            {
                // Outside the field: Place random maze walls
                // We use a simple seed or pattern here
                if ((x % 2 == 0 && z % 2 == 0) || x == 0 || x == MAP_WIDTH - 1 || z == 0 || z == MAP_HEIGHT - 1)
                {
                    shouldPlaceWall = true;
                }
            }

            if (shouldPlaceWall)
            {
                glm::vec3 pos(
                    (x - MAP_WIDTH / 2.0f) * wallSize,
                    wallHeight / 2.0f - 0.5f,
                    (z - MAP_HEIGHT / 2.0f) * wallSize);

                walls.push_back(new Wall(pos, glm::vec3(wallSize, wallHeight, wallSize)));
            }
        }
    }
}