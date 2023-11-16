#version 330 core
in vec2 TexCoord;
in vec4 position;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(
    texture(texture1, TexCoord),
    texture(texture2, vec2(-2.0 * TexCoord.x, 2.0 * TexCoord.y)),
    0.5
  ) * (position + 1) / 2;
}
