#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor; // the colour of this object
uniform vec3 lightColor; // the color of the light

void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0);
}