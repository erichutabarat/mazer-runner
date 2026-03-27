#include "Objects.h"
#include <iostream>

Objects::Objects(float vertices[], size_t size, const char *vShaderCode, const char *fShaderCode)
{
    m_VertexCount = size / (3 * sizeof(float)); // Assuming 3 floats (x,y,z) per vertex

    // 1. Compile Shaders & Link Program
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vShaderCode);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fShaderCode);

    m_Program = glCreateProgram();
    glAttachShader(m_Program, vs);
    glAttachShader(m_Program, fs);
    glLinkProgram(m_Program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // 2. Setup GPU Buffers (VAO/VBO)
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to read the vertex data (Location 0, 3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Objects::draw() const
{
    glUseProgram(m_Program);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
}

Objects::~Objects()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteProgram(m_Program);
}

unsigned int Objects::compileShader(unsigned int type, const char *source)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    // (Optional: Add error checking here)
    return id;
}