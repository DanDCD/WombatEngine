#version 330 core
// vertex shaders are (data for 1 vert) => positional data for 1 vert

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos; // position of the fragment in world space
out vec3 Normal; // normal
out vec2 Texcoord; // the texcoord for specular and diffusion maps

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalModel; // the normal model matrix (transformation matrix for normals into world space)

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0)); // forwards the world position to the fragment shader
    Normal = normalModel * aNormal; // forwards the normal (in world space) to the fragment shader
    Texcoord = aTexCoord; // forwards the texture coord to the fragment shader

    // work out the position of this vertex with respect to: project * camera view * world position * local coord
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
}
