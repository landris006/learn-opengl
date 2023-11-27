#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <iostream>

class Transform {
 private:
  glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 _right = glm::vec3(1.0f, 0.0f, 0.0f);

  float _pitch = 0.0f;
  float _yaw = 0.0f;
  float _roll = 0.0f;

 public:
  Transform() {}

  glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

  glm::vec3 front() const {
    return _front;
  }
  glm::vec3 right() const {
    return _right;
  }
  glm::vec3 up() const {
    return _up;
  }
  float yaw() const {
    return _yaw;
  }
  float pitch() const {
    return _pitch;
  }
  float roll() const {
    return _roll;
  }

  void setEuler(float pitch, float yaw, float roll) {
    _pitch = pitch;
    _yaw = yaw;
    _roll = roll;

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    newFront.y = sin(glm::radians(_pitch));
    newFront.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

    _front = glm::normalize(newFront);
    _right = glm::normalize(glm::cross(_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    _up = glm::normalize(glm::cross(_right, _front));
  }
};
