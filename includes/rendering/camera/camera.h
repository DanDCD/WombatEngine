#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera();
private:

    /// @brief the position of the camera in world space
    glm::vec3 cameraPos;

    /// @brief the direction the camera is facing
    glm::vec3 cameraFront;

    /// @brief the direction that is 'up' relative to the camera
    glm::vec3 cameraUp; 

    /// @brief the direction that is 'right' relative to the camera
    glm::vec3 cameraRight;

    /// @brief euler angle
    float yaw;
};