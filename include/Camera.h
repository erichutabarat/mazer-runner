#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "miniaudio.h"
#include <vector>

class Wall;

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    ~Camera();

    glm::mat4 getViewMatrix() const;
    void processKeyboard(GLFWwindow *window, float deltaTime, std::vector<Wall *> &walls);

    // Camera Attributes
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    float m_Yaw;
    float m_Pitch;
    float m_MovementSpeed;

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

private:
    void updateCameraVectors();
    // control sensitivity:
    float m_MouseSensitivity = 0.1f;

    ma_engine m_AudioEngine;
    ma_sound m_FootstepSound;
    bool m_IsSoundLoaded = false;
};

#endif