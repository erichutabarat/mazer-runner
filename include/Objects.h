#ifndef OBJECTS_H
#define OBJECTS_H

#include <GL/glew.h> // Include GLEW for OpenGL types
#include <string>

class Objects
{
public:
    // pass vertex count so the class knows how many points to draw
    Objects(float vertices[], size_t size, const char *vShaderCode, const char *fShaderCode);
    ~Objects();

    void draw() const;

private:
    unsigned int m_VAO, m_VBO, m_Program;
    int m_VertexCount;

    // Helper to compile shaders
    unsigned int compileShader(unsigned int type, const char *source);
};

#endif