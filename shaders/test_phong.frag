#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

in vec3 FragPos; // the position of the fragment in world space (interpolated from the vertex shader for each fragment between vertices)
in vec3 Normal; // the normal of the fragment 
in vec2 Texcoord; // the coords (interpolated) for the diffuse/specular maps corresponding to this fragment

uniform vec3 viewPos; // the world space coords of the viewer (i.e active camera)
uniform Material material; // the material of the object
uniform Light light; // light properties


void main()
{
    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, Texcoord)); // the ambient light color


    // diffuse lighting
    vec3 norm = normalize(Normal); // the normal vector
    vec3 fragPosToLightDir = normalize(light.position - FragPos); // the direction from the fragment to the light source
    float diff = max(dot(norm, fragPosToLightDir), 0.0); // the diffuse factor - this is the dot product of the normal and the direction to the light source
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuseMap, Texcoord)); // the diffuse light color


    // specular lighting
    vec3 fragToViewDir = normalize(viewPos - FragPos);
    vec3 lightToFragPosDir = -fragPosToLightDir;
    vec3 reflectDir = reflect(lightToFragPosDir, norm);
    float spec = pow(max(dot(fragToViewDir, reflectDir), 0.0), material.shininess); // spec factor is how close the angle of frag to view is to the angle of reflection from light
    vec3 specular = spec * light.specular * vec3(texture(material.specularMap, Texcoord));


    // result color
    vec3 result = (ambient + diffuse + specular); // the result of the ambient light
    FragColor = vec4(result, 1.0);
}