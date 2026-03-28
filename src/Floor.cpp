#include "Floor.h"
#include <glm/gtc/type_ptr.hpp>

// Define the static data here so it doesn't clutter main
const char *fVertexShader = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "layout (location = 1) in vec2 aTexCoord;\n"
                            "out vec2 TexCoord;\n"
                            "uniform mat4 model; uniform mat4 view; uniform mat4 projection;\n" // Added model uniform
                            "void main() {\n"
                            "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n" // Use model here!
                            "    TexCoord = aTexCoord;\n"
                            "}\0";

const char *fFragmentShader = "#version 330 core\n"
                              "out vec4 FragColor;\n"
                              "in vec2 TexCoord;\n"
                              "uniform sampler2D texture1;\n"
                              "uniform float tiling;\n"
                              "void main() { FragColor = texture(texture1, TexCoord * tiling); }\n\0";

Floor::Floor() : Objects(nullptr, 0, fVertexShader, fFragmentShader)
{
    float vertices[] = {
        // Positions             // Texture Coords (UV)
        -50.0f, -0.5f, 50.0f, 0.0f, 10.0f, // Top Left
        -50.0f, -0.5f, -50.0f, 0.0f, 0.0f, // Bottom Left
        50.0f, -0.5f, -50.0f, 10.0f, 0.0f, // Bottom Right

        -50.0f, -0.5f, 50.0f, 0.0f, 10.0f, // Top Left
        50.0f, -0.5f, -50.0f, 10.0f, 0.0f, // Bottom Right
        50.0f, -0.5f, 50.0f, 10.0f, 10.0f  // Top Right
    };

    // 1. Re-bind the VAO and VBO created by the base class
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // 2. Upload the new data (5 floats per vertex)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. Clear any old attribute pointers and set new ones
    // Position (Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Texture Coords (Location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_VertexCount = 6;

    // Unbind to stay clean
    glBindVertexArray(0);
}

// Change the signature to match the header
void Floor::render(glm::mat4 view, glm::mat4 projection, unsigned int textureID)
{
    glUseProgram(getProgram()); // Use program FIRST

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID); // Use the ID passed from main

    // Tell the shader that "texture1" should look at Texture Unit 0
    glUniform1f(glGetUniformLocation(getProgram(), "tiling"), 10.0f);

    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    this->draw();
}