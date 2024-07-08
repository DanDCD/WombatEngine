#include "rendering/camera/camera.h"
#include "globals/world_consts.h"

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

void Camera::processKeyboard(Movement direction, float deltaTime) {}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {}

void Camera::processMouseScroll(float yoffset) {}