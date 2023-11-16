#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <lib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char *argv[]) {
  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
  vec = trans * vec;

  std::cout << vec.x << vec.y << vec.z << std::endl;

  run();
}
