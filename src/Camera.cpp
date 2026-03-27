#include <GL/glew.h>
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

    // 1. Create a "Grounded" Front Vector
    // We take the current Front vector, but set Y to 0 so we only move on the XZ plane.
    glm::vec3 groundedFront = glm::normalize(glm::vec3(m_Front.x, 0.0f, m_Front.z));

    // 2. Calculate the Right vector (already horizontal, so no changes needed)
    glm::vec3 right = glm::normalize(glm::cross(m_Front, m_Up));

    // 3. Handle Inputs
    // Forward / Backward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_Position += groundedFront * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_Position -= groundedFront * velocity;

    // Strafe Left / Right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_Position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_Position += right * velocity;

    // 4. THE CONDITION CHECKS
    float floorLevel = -0.5f; // Matches your Floor.cpp
    float eyeHeight = 1.5f;   // Distance from floor to player "eyes"

    // Hard-lock the height so the player cannot fly or sink
    m_Position.y = floorLevel + eyeHeight;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
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