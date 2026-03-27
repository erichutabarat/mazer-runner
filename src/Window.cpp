#include <GL/glew.h>
#include "Window.h"
#include <iostream>

#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const std::string &title)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // --- CONFIGS ---
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // disable resizing to avoid complications with viewport and aspect ratio
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // --- BORDERLESS FULLSCREEN LOGIC ---
    // 1. Get the primary monitor's video mode (resolution/refresh rate)
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    // 2. Set hints to make it "Windowed Fullscreen"
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    // 3. Create the window using the monitor's width/height
    m_window = glfwCreateWindow(mode->width, mode->height, title.c_str(), NULL, NULL);

    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE; // Necessary for Core Profile
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
    }

    // print
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::clear(float r, float g, float b, float a) const
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::update() const
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}