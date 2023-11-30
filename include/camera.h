#pragma once

#include "./transform.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/matrix.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/gtc/quaternion.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera : public Transform {
 private:
 public:
  Camera() {}

  float vfov = 45.0f;
  float viewportWidth = 1600.0f;
  float viewportHeight = 800.0f;

  glm::mat4 getViewMatrix() const {
    return glm::lookAt(translation, translation + front(), up());
  }

  glm::mat4 getProjectionMatrix() const {
    return glm::perspective(
        glm::radians(vfov), viewportWidth / viewportHeight, 0.1f, 100.0f);
  }
};
