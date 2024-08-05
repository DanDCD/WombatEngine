#version 330 core
out vec4 FragColor;

uniform vec3 objectColor; // the colour of this object

void main()
{
    FragColor = vec4(objectColor, 1.0);
}