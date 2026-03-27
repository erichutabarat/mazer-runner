#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(5.0f)
{
    m_Position = position;
    m_Up = up;
    m_Yaw = yaw;
    m_Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::processKeyboard(GLFWwindow *window, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Position += m_Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Position -= m_Front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * velocity;
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector based on Euler Angles (Yaw/Pitch)
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
}