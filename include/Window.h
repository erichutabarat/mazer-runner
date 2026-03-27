#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
    Window(int width, int height, const std::string &title);
    ~Window();

    bool shouldClose() const;
    void update() const;
    void clear(float r, float g, float b, float a) const;

private:
    GLFWwindow *m_window;
};

#endif