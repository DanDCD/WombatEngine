#version 330 core

in vec3 FragPos; // the position of the fragment in world space (interpolated from the vertex shader for each fragment between vertices)
in vec3 Normal; // the normal of the fragment 

out vec4 FragColor;
  
uniform vec3 objectColor; // the colour of this object
uniform vec3 lightColor; // the color of the light
uniform vec3 lightPos; // the position of the light source
uniform vec3 viewPos; // the world space coords of the viewer (i.e active camera)


void main()
{
    // ambient lighting
    float ambientStrength = 0.1; // the strength of the ambient light
    vec3 ambient = ambientStrength * lightColor; // the ambient light color


    // diffuse lighting
    vec3 norm = normalize(Normal); // the normal vector
    vec3 fragPosToLightDir = normalize(lightPos - FragPos); // the direction from the fragment to the light source

    float diff = max(dot(norm, fragPosToLightDir), 0.0); // the diffuse factor - this is the dot product of the normal and the direction to the light source
    // as both are normalized, this will be the cosine of the angle between them 
    // - if the angle is 90 degrees, the dot product will be 0 and if the angle is 0 degrees, the dot product will be 1
    // if the dot product is negative, the light is behind the object and we don't want to add any light

    vec3 diffuse = diff * lightColor; // the diffuse light color


    // specular lighting
    float specularStrength = 0.5;
    int shininess = 32;
    vec3 fragToViewDir = normalize(viewPos - FragPos);
    vec3 lightToFragPosDir = -fragPosToLightDir;
    vec3 reflectDir = reflect(lightToFragPosDir, norm);
    float spec = pow(max(dot(fragToViewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;


    // result color
    vec3 result = (ambient + diffuse + specular) * objectColor; // the result of the ambient light
    FragColor = vec4(result, 1.0);
    // debug normals
    // FragColor = vec4(norm, 1.0);
}