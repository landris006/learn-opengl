#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
 private:
  unsigned int id;

 public:
  Shader(const char *vertexPath, const char *fragmentPath);

  int getId() const;
  void use() const;
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMatrix4(const std::string &name, glm::mat4 value) const;
};
