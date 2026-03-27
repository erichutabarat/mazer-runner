#include <GL/glew.h>
#include "Camera.h"
#include <iostream>
#include "Wall.h"
#include <vector>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(5.0f), m_IsSoundLoaded(false)
{
    m_Position = position;
    m_Up = up;
    m_Yaw = yaw;
    m_Pitch = pitch;

    if (ma_engine_init(NULL, &m_AudioEngine) == MA_SUCCESS)
    {
        // FIX: Path changed from "sounds" to "sound" to match your folder
        if (ma_sound_init_from_file(&m_AudioEngine, "assets/sound/walking.wav", MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_DECODE, NULL, NULL, &m_FootstepSound) == MA_SUCCESS)
        {
            ma_sound_set_looping(&m_FootstepSound, MA_TRUE);
            m_IsSoundLoaded = true;
            std::cout << "[AUDIO] Success: walking.wav loaded!" << std::endl;
        }
        else
        {
            std::cerr << "[AUDIO] Error: Could not find assets/sound/walking.wav" << std::endl;
        }
    }
    updateCameraVectors();
}

Camera::~Camera()
{
    if (m_IsSoundLoaded)
        ma_sound_uninit(&m_FootstepSound);
    ma_engine_uninit(&m_AudioEngine);
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::processKeyboard(GLFWwindow *window, float deltaTime, std::vector<Wall *> &walls)
{
    // 1. Declare it ONCE at the top
    bool isMoving = false;
    float velocity = m_MovementSpeed * deltaTime;

    glm::vec3 groundedFront = glm::normalize(glm::vec3(m_Front.x, 0.0f, m_Front.z));
    glm::vec3 right = glm::normalize(glm::cross(m_Front, m_Up));

    // Save old position for collision "undo"
    glm::vec3 oldPos = m_Position;

    // 2. Handle Inputs (Updates isMoving to true)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_Position += groundedFront * velocity;
        isMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_Position -= groundedFront * velocity;
        isMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_Position -= right * velocity;
        isMoving = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_Position += right * velocity;
        isMoving = true;
    }

    // 3. Wall Collision Logic
    for (Wall *wall : walls)
    {
        if (wall->isColliding(m_Position, 0.3f))
        {
            m_Position = oldPos; // Hit a wall! Reset position.
            // If we hit a wall and can't move, we might want to stop the sound
            // but usually, players like the "running against a wall" sound.
            break;
        }
    }

    // 4. Audio Logic (DO NOT re-declare bool isMoving here!)
    if (m_IsSoundLoaded)
    {
        if (isMoving)
        {
            if (!ma_sound_is_playing(&m_FootstepSound))
                ma_sound_start(&m_FootstepSound);
        }
        else
        {
            if (ma_sound_is_playing(&m_FootstepSound))
            {
                ma_sound_stop(&m_FootstepSound);
                ma_sound_seek_to_pcm_frame(&m_FootstepSound, 0);
            }
        }
    }

    // 5. Final Floor Lock
    m_Position.y = 1.0f;
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