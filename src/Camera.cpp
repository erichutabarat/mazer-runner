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

void Camera::processKeyboard(GLFWwindow *window, float deltaTime,
                             std::vector<Wall *> &walls,
                             std::vector<Item> &worldItems,
                             std::vector<Item> &inventory)
{
    // --- 1. PHYSICS CONSTANTS ---
    const float GRAVITY = -20.0f;
    const float JUMP_FORCE = 7.5f;
    const float floorLevel = -0.5f; // Floor Y from Floor.cpp

    // --- 2. DYNAMIC STATE ---
    float targetEyeHeight = 1.5f; // Normal height
    float velocity = m_MovementSpeed * deltaTime;
    bool isMoving = false;

    // --- 2.5 SPRINTING ---
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        velocity *= 1.5f;
    }

    // --- 2.75 CROUCHING ---
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        velocity *= 0.5f;       // Slow down
        targetEyeHeight = 0.7f; // Lower target height
    }

    // SMOOTH CROUCHING (Optional but recommended)
    // If you prefer instant, just use: m_CurrentEyeHeight = targetEyeHeight;
    static float m_CurrentEyeHeight = 1.5f;
    m_CurrentEyeHeight = glm::mix(m_CurrentEyeHeight, targetEyeHeight, deltaTime * 10.0f);

    // Calculate the actual ground level for this frame
    float dynamicGroundY = floorLevel + m_CurrentEyeHeight;

    // --- 3. HORIZONTAL MOVEMENT ---
    glm::vec3 groundedFront = glm::normalize(glm::vec3(m_Front.x, 0.0f, m_Front.z));
    glm::vec3 right = glm::normalize(glm::cross(m_Front, m_Up));
    glm::vec3 oldPos = m_Position;

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

    // --- INTERACTION (E) ---
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        for (auto &item : worldItems)
        {
            if (!item.isPickedUp)
            {
                if (glm::distance(m_Position, item.position) < 1.8f)
                {
                    item.isPickedUp = true;
                    inventory.push_back(item);
                    std::cout << "[GAME] Picked up: " << item.name << std::endl;
                }
            }
        }
    }

    // --- 4. VERTICAL PHYSICS ---
    m_VerticalVelocity += GRAVITY * deltaTime;
    m_Position.y += m_VerticalVelocity * deltaTime;
    m_IsGrounded = false;

    // --- 5. WALL COLLISION ---
    for (Wall *wall : walls)
    {
        if (wall->isColliding(m_Position, 0.3f))
        {
            float wallTop = wall->getPosition().y + (wall->getSize().y / 2.0f);
            // Check feet against wall top using the current height
            float playerFeet = m_Position.y - m_CurrentEyeHeight;

            if (m_VerticalVelocity <= 0.0f && playerFeet >= (wallTop - 0.2f))
            {
                m_Position.y = wallTop + m_CurrentEyeHeight;
                m_VerticalVelocity = 0.0f;
                m_IsGrounded = true;
            }
            else
            {
                m_Position.x = oldPos.x;
                m_Position.z = oldPos.z;
            }
        }
    }

    // --- 6. FLOOR COLLISION ---
    // Use dynamicGroundY so the camera height actually changes
    if (m_Position.y <= dynamicGroundY)
    {
        m_Position.y = dynamicGroundY;
        m_VerticalVelocity = 0.0f;
        m_IsGrounded = true;
    }

    // --- 7. JUMP INPUT ---
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && m_IsGrounded)
    {
        m_VerticalVelocity = JUMP_FORCE;
        m_IsGrounded = false;
    }

    // --- 8. AUDIO LOGIC ---
    if (m_IsSoundLoaded)
    {
        if (isMoving && m_IsGrounded)
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