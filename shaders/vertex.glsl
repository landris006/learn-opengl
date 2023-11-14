#version 330 core
layout(location = 0) in vec3 aPos;
uniform float offsetX;

out vec4 position;

void main() {
  gl_Position = vec4(aPos.x + offsetX, aPos.y, aPos.z, 1.0);
  position = gl_Position;
}
