#version 330 core

out vec3 FragPos; // position of the fragment in world space
out vec3 Normal; // normal

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalModel; // the normal model matrix (transformation matrix for normals into world space)

void main()
{
    // work out the position of this pixel with respect to: project * camera view * world position * local coord
    gl_Position = projection * view * model * vec4(aPos, 1.0); 

    FragPos = vec3(model * vec4(aPos, 1.0)); // forwards the world position to the fragment shader
    Normal = normalModel * aNormal; // forwards the normal (in world space) to the fragment shader
}
