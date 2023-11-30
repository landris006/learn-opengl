#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoord;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;

void main() {
    vec3 diffuseColor = texture(material.diffuseMap, TexCoord).rgb;

    vec3 ambient = light.ambient * diffuseColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * diffuseColor;

    vec3 specularColor = texture(material.specularMap, TexCoord).rgb;
    vec3 viewDir = normalize(vec3(0.0) - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * specularColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
