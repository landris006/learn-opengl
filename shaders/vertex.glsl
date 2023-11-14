#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform float offsetX;

out vec4 position;
out vec2 TexCoord;

void main() {
  gl_Position = vec4(aPos.x + offsetX, aPos.y, aPos.z, 1.0);
  position = gl_Position;
  TexCoord = aTexCoord;
}
