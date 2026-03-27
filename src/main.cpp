#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Objects.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Corrected Vertex Shader with Matrices for 3D
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "\n"
                                 "uniform mat4 model;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "\n"
                                 "void main() {\n"
                                 "   // OpenGL math: Right-to-Left multiplication\n"
                                 "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                 "}\0";

// Simple Fragment Shader to color the floor (Light Gray)
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main() {\n"
                                   "   FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);\n"
                                   "}\n\0";

int main()
{
    Window gameWindow(800, 600, "Maze Runner");

    float floorVertices[] = {
        -10.0f, -0.5f, -10.0f, 10.0f, -0.5f, -10.0f, 10.0f, -0.5f, 10.0f,
        -10.0f, -0.5f, -10.0f, 10.0f, -0.5f, 10.0f, -10.0f, -0.5f, 10.0f};

    // 1. Create the object first
    Objects floor(floorVertices, sizeof(floorVertices), vertexShaderSource, fragmentShaderSource);

    while (!gameWindow.shouldClose())
    {
        gameWindow.clear(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        // 2. Tell OpenGL WHICH shader we are using before sending matrices
        unsigned int program = floor.getProgram();
        glUseProgram(program);

        // 3. Create the 3D Math (Matrices)
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        // Look from slightly above (y=2) and back (z=5)
        view = glm::translate(view, glm::vec3(0.0f, -1.0f, -5.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // 4. Send the math to the GPU
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // 5. Finally, draw
        floor.draw();

        gameWindow.update();
    }
    return 0;
}