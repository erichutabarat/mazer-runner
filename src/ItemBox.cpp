#include "ItemBox.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

// Shader that takes a custom color uniform
const char *itemVS = "#version 330 core\n"
                     "layout (location = 0) in vec3 aPos;\n"
                     "uniform mat4 model; uniform mat4 view; uniform mat4 projection;\n"
                     "void main() { gl_Position = projection * view * model * vec4(aPos, 1.0); }";

const char *itemFS = "#version 330 core\n"
                     "out vec4 FragColor;\n"
                     "uniform vec3 itemColor;\n"
                     "void main() { FragColor = vec4(itemColor, 1.0); }";

ItemBox::ItemBox() : Objects(nullptr, 0, itemVS, itemFS)
{
    float vertices[] = {
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f, // Standard Cube
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        -0.5f,
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    m_VertexCount = 36;
}

void ItemBox::render(glm::vec3 position, float rotation, glm::vec3 color, glm::mat4 view, glm::mat4 projection)
{
    glUseProgram(getProgram());

    // Create the "Floating" effect using a Sine wave
    float floatOffset = sin(glfwGetTime() * 2.0f) * 0.1f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position + glm::vec3(0, floatOffset, 0));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(0.3f)); // Make it small

    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(glGetUniformLocation(getProgram(), "itemColor"), 1, glm::value_ptr(color));

    this->draw();
}