#include "root/components/transform_component.h"

Wombat::ECS::TransformComponent::TransformComponent()
    : position(0.0f, 0.0f, 0.0f)
{
}

const glm::mat4 &Wombat::ECS::TransformComponent::getTransformMatrix() const
{
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, position);
    // transform = glm::rotate(transform, )
}

const glm::vec3 &Wombat::ECS::TransformComponent::getPosition() const
{
    return position;
}

void Wombat::ECS::TransformComponent::setPosition(glm::vec3 new_position)
{
    position = new_position;
}

void Wombat::ECS::TransformComponent::translate(const glm::vec3 &offset)
{
    position += offset;
}