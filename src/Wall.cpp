#include "Wall.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char *wallVS = "#version 330 core\n"
                     "layout (location = 0) in vec3 aPos;\n"
                     "layout (location = 1) in vec2 aTexCoord;\n"
                     "out vec2 TexCoord;\n"
                     "uniform mat4 model; uniform mat4 view; uniform mat4 projection;\n"
                     "void main() {\n"
                     "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                     "    TexCoord = aTexCoord;\n"
                     "}";

const char *wallFS = "#version 330 core\n"
                     "out vec4 FragColor;\n"
                     "in vec2 TexCoord;\n"
                     "uniform sampler2D wallTexture;\n"
                     "void main() { FragColor = texture(wallTexture, TexCoord); }";

Wall::Wall(glm::vec3 position, glm::vec3 size)
    : Objects(nullptr, 0, wallVS, wallFS), m_Position(position), m_Size(size)
{
    float vertices[] = {
        // Positions          // Texture Coords
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        // Left face
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        // Top face
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    glBindVertexArray(m_VAO); // Ensure we are editing THIS wall's VAO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position: 3 floats, Stride is 5 * sizeof(float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Texture: 2 floats, Stride is 5 * sizeof(float), Offset is 3 floats
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_VertexCount = 36;
    glBindVertexArray(0);
}

bool Wall::isColliding(glm::vec3 playerPos, float playerRadius)
{
    // AABB (Axis-Aligned Bounding Box) Collision Detection
    // Check if player's X/Z is within the wall's box boundaries
    bool collisionX = playerPos.x + playerRadius >= m_Position.x - m_Size.x / 2.0f &&
                      m_Position.x + m_Size.x / 2.0f >= playerPos.x - playerRadius;

    bool collisionZ = playerPos.z + playerRadius >= m_Position.z - m_Size.z / 2.0f &&
                      m_Position.z + m_Size.z / 2.0f >= playerPos.z - playerRadius;

    // We only care about X and Z for basic wall blocking
    return collisionX && collisionZ;
}

void Wall::render(glm::mat4 view, glm::mat4 projection, unsigned int textureID)
{
    glUseProgram(getProgram());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // IMPORTANT: Tell the shader to use Texture Unit 0
    int texLoc = glGetUniformLocation(getProgram(), "wallTexture");
    glUniform1i(texLoc, 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_Position);
    model = glm::scale(model, m_Size);

    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    this->draw();
}