#include "Wall.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char *wallVS = "#version 330 core\n"
                     "layout (location = 0) in vec3 aPos;\n"
                     "uniform mat4 model; uniform mat4 view; uniform mat4 projection;\n"
                     "void main() { gl_Position = projection * view * model * vec4(aPos, 1.0); }";

const char *wallFS = "#version 330 core\n"
                     "out vec4 FragColor;\n"
                     "void main() { FragColor = vec4(0.6f, 0.3f, 0.1f, 1.0f); }"; // Brownish wall

Wall::Wall(glm::vec3 position, glm::vec3 size)
    : Objects(nullptr, 0, wallVS, wallFS), m_Position(position), m_Size(size)
{

    float vertices[] = {
        // Back face
        -0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        -0.5f,
        // Front face
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
        // Left face
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
        // Right face
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
        // Bottom face
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
        // Top face
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

void Wall::render(glm::mat4 view, glm::mat4 projection)
{
    glUseProgram(getProgram());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_Position);
    model = glm::scale(model, m_Size);

    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    this->draw();
}

bool Wall::isColliding(glm::vec3 playerPos, float playerRadius)
{
    // Check if player's X/Z is within the wall's box boundaries
    bool collisionX = playerPos.x + playerRadius >= m_Position.x - m_Size.x / 2 &&
                      m_Position.x + m_Size.x / 2 >= playerPos.x - playerRadius;
    bool collisionZ = playerPos.z + playerRadius >= m_Position.z - m_Size.z / 2 &&
                      m_Position.z + m_Size.z / 2 >= playerPos.z - playerRadius;
    return collisionX && collisionZ;
}