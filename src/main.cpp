#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Objects.h"

// Vertex Shader: Positions the triangle's corners
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main() {\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// Fragment Shader: Colors the pixels (This one makes it Orange)
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main() {\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

int main()
{
    Window gameWindow(800, 600, "Maze Runner");

    float triangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    // Assuming you still have your shader strings from earlier
    Objects triangle(triangleVertices, sizeof(triangleVertices), vertexShaderSource, fragmentShaderSource);

    while (!gameWindow.shouldClose())
    {
        gameWindow.clear(0.1f, 0.1f, 0.1f, 1.0f);

        triangle.draw(); // Just one line to draw!

        gameWindow.update();
    }

    return 0;
}