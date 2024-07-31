#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor; // the colour of this object
uniform vec3 lightColor; // the color of the light

void main()
{
    // ambient lighting
    float ambientStrength = 0.1; // the strength of the ambient light
    vec3 ambient = ambientStrength * lightColor; // the ambient light color

    


    vec3 result = ambient * objectColor; // the result of the ambient light
    FragColor = vec4(result, 1.0);
}