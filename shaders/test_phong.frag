#version 330 core
out vec4 FragColor;

// A directional light (e.g. the sun), where light only has direction - not position
struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// A point light (e.g. a light bulb), where light is emitted in all directions from a position
struct PointLight {
    vec3 position;

    float constant; // the constant factor in attenuation
    float linear; // the linear factor in attenuation
    float quadratic; // the quadratic factor in attenuation

    // values used for attenuation
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


uniform DirectionalLight dirLight; // the directional light data for this scene

#define NR_POINT_LIGHTS 1 // the number of point lights
uniform PointLight pointLights[NR_POINT_LIGHTS]; // the uniform for the point lights data


// calculate phong lighting for a directional light and return resulting RGB for frag
vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 normal, vec3 fragToViewDir)
{
    // calculate normalised light directions (both forwards and reverse)
    vec3 lightDir = normalize(dirLight.direction);
    vec3 revLightDir = -lightDir;

    // diffuse shading factor (how perpendicular the angle is between the light and the fragment)
    float diffFactor = max(dot(normal, revLightDir), 0.0);

    // specular shading factor (how close the view direction is to the natural reflection of the light)
    vec3 reflectDir = reflect(lightDir, normal);
    float specFactor = pow(max(dot(fragToViewDir, reflectDir), 0.0), material.shininess);

    // calculate ambient result
    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuseMap, Texcoord));
    // calculate diffuse result
    vec3 diffuse = dirLight.diffuse * diffFactor * vec3(texture(material.diffuseMap, Texcoord));
    // calculate specular result
    vec3 specular = dirLight.specular * specFactor * vec3(texture(material.specularMap, Texcoord));
    
    // calculate final result
    return (ambient + diffuse + specular);
}

// calculate phong lighting for a point light and return resulting RGB for frag
vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // calculate normalised directions between this frag and the lightsource
    vec3 fragToLightDir = normalize(pointLight.position - fragPos);
    vec3 lightToFragDir = -fragToLightDir;

    // diffuse shading factor (how perpendicular the angle is between the light and the fragment)
    float diffFactor = max(dot(normal, fragToLightDir), 0.0);

    // specular shading factor (how close the view direction is to the natural reflection of the light)
    vec3 reflectDir = reflect(lightToFragDir, normal);
    float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // calculate ambient result
    vec3 ambient = pointLight.ambient * vec3(texture(material.diffuseMap, Texcoord));
    // calculate diffuse result
    vec3 diffuse = pointLight.diffuse * diffFactor * vec3(texture(material.diffuseMap, Texcoord));
    // calculate specular result
    vec3 specular = pointLight.specular * specFactor * vec3(texture(material.specularMap, Texcoord));

    // calculate attenuation
    float distanceToLight = length(pointLight.position - fragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distanceToLight + pointLight.quadratic * distanceToLight * distanceToLight);

    // modulate by attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}


void main()
{
    vec3 normal = normalize(Normal);
    vec3 fragToViewDir = normalize(viewPos - FragPos);

    // process directional light
    vec3 result = CalculateDirectionalLight(dirLight, normal, fragToViewDir);

    // process point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalculatePointLight(pointLights[i], normal, FragPos, fragToViewDir);

    // TODO: process spot lights

    FragColor = vec4(result, 1.0);
}