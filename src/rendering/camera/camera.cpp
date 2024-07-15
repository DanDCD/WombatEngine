#include "rendering/camera/camera.h"
#include "rendering/render_consts.h"
#include "algorithm"
#include <iostream>

CameraParams::CameraParams()
    : cameraPos(0.0f, 0.0f, 0.0f),
      yaw(0.0f), pitch(0.0f), movementSpeed(2.5f),
      mouseSensitivity(0.1f), zoom(45.0f)
{
    updateDirections();
}

CameraParams::CameraParams(const glm::vec3 &pos,
                           float yaw, float pitch,
                           float speed, float sensitivity, float zoom)
    : cameraPos(pos), yaw(yaw),
      pitch(pitch), movementSpeed(speed), mouseSensitivity(sensitivity), zoom(zoom)
{
    updateDirections();
}

void CameraParams::updateDirections()
{
    // calculate front
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    // calculate right and up
    cameraRight = glm::normalize(glm::cross(cameraFront, WORLD_UP));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

Camera::Camera(CameraParams &cameraParams)
    : cameraParams(cameraParams)
{
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(cameraParams.cameraPos, cameraParams.cameraPos + cameraParams.cameraFront, cameraParams.cameraUp);
}

void Camera::processKeyboard(Movement direction, float deltaTime)
{
    float velocity = cameraParams.movementSpeed * deltaTime;
    if (direction == Movement::FORWARD)
        cameraParams.cameraPos += glm::normalize(cameraParams.cameraFront) * velocity;
    if (direction == Movement::BACKWARD)
        cameraParams.cameraPos -= glm::normalize(cameraParams.cameraFront) * velocity;
    if (direction == Movement::LEFT)
        cameraParams.cameraPos -= glm::normalize(cameraParams.cameraRight) * velocity;
    if (direction == Movement::RIGHT)
        cameraParams.cameraPos += glm::normalize(cameraParams.cameraRight) * velocity;
    if (direction == Movement::UP)
        cameraParams.cameraPos += glm::normalize(WORLD_UP) * velocity;
    if (direction == Movement::DOWN)
        cameraParams.cameraPos -= glm::normalize(WORLD_UP) * velocity;
    // we don't need to update dirs as the pitch/yaw does not change here only position
    // (so left/right/up/down dirs are constant)
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    // Scale mouse movement by sensitivity
    xoffset *= cameraParams.mouseSensitivity;
    yoffset *= cameraParams.mouseSensitivity;

    cameraParams.yaw += xoffset;
    cameraParams.pitch += yoffset;

    if (constrainPitch)
    {
        // Constrain pitch to avoid flipping
        cameraParams.pitch = std::max(cameraParams.pitch, -89.0f);
        cameraParams.pitch = std::min(cameraParams.pitch, 89.0f);
    }
    cameraParams.updateDirections(); // we do have to update dirs here, as we are modifying pitch and yaw
}

void Camera::processMouseScroll(float yoffset)
{
    cameraParams.zoom -= (float)yoffset;
    cameraParams.zoom = std::max(cameraParams.zoom, 1.0f);
    cameraParams.pitch = std::min(cameraParams.pitch, 45.0f);
}