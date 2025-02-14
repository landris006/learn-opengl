#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoord;

void main() {
    // TODO: toggle Goraud shading
    // TODO: should be a uniform for better performance
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    LightPos = vec3(view * vec4(lightPos, 1.0));
    TexCoord = aTexCoord;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
