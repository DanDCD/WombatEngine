#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Wombat
{
    namespace ECS
    {

        /// @brief This component is used to describe an entity's scale, rotation, and position in world space
        struct TransformComponent
        {
        public:

            TransformComponent();


            /// @brief returns the matrix representation fo this transform
            /// @return 
            const glm::mat4 &getTransformMatrix() const;

            /// @brief
            /// @return
            const glm::vec3 &getPosition() const;

            /// @brief
            /// @param new_position
            void setPosition(glm::vec3 new_position);

            /// @brief
            /// @param offset
            void translate(const glm::vec3 &offset);

        private:
            /// @brief a helper function to update the directions when yaw/ changes
            void update_directions();


            /// @brief the 3D position in world-space
            glm::vec3 position;

            /// @brief the euler angle for rotating left/right along the y-axis
            float yaw;

            /// @brief the euler angle for rotating up/down along the x-axis
            float pitch;

            /// @brief 
            float roll;

            /// @brief a scaling factor - where a value of 1.0 equates to 100%
            glm::vec3 scale;

            /// @brief unit vector denoting the 'front' direction of a transform
            glm::vec3 front;

            /// @brief unit vector denoting the 'up' direction of a transform
            glm::vec3 up;

            /// @brief unit vector denoting the 'right' direction of a transform
            glm::vec3 right;
        };
    }
}