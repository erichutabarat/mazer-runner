#include "Floor.h"
#include <glm/gtc/type_ptr.hpp>

// Define the static data here so it doesn't clutter main
const char *fVertexShader = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "uniform mat4 model; uniform mat4 view; uniform mat4 projection;\n"
                            "void main() { gl_Position = projection * view * model * vec4(aPos, 1.0); }\0";

const char *fFragmentShader = "#version 330 core\n"
                              "out vec4 FragColor;\n"
                              "void main() { FragColor = vec4(0.3f, 0.3f, 0.3f, 1.0f); }\n\0";

Floor::Floor() : Objects(nullptr, 0, fVertexShader, fFragmentShader)
{
    float vertices[] = {
        -10.0f, -0.5f, -10.0f, 10.0f, -0.5f, -10.0f, 10.0f, -0.5f, 10.0f,
        -10.0f, -0.5f, -10.0f, 10.0f, -0.5f, 10.0f, -10.0f, -0.5f, 10.0f};

    // We need to update the Objects class to handle this,
    // but for now, let's just re-bind the data manually in the constructor
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    m_VertexCount = 6;
}

void Floor::render(glm::mat4 view, glm::mat4 projection)
{
    glUseProgram(getProgram());

    glm::mat4 model = glm::mat4(1.0f);

    // Send matrices to shader
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    this->draw(); // Call the base class draw
}