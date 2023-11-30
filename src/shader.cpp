#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstdio>
#include <filesystem>
#include <format>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <shader.h>
#include <sstream>
#include <string>

using glm::mat4;

unsigned int compileShader(unsigned int shaderType, const char **shaderSource) {
  unsigned int shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, shaderSource, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << std::format(
                     "ERROR::SHADER::{}::COMPILATION_FAILED\n", shaderType)
              << infoLog << std::endl;
  }

  return shader;
}

unsigned int
createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  return shaderProgram;
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // 1. retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    std::stringstream vShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    vShaderFile.close();
    vertexCode = vShaderStream.str();

    fShaderFile.open(fragmentPath);
    std::stringstream fShaderStream;
    fShaderStream << fShaderFile.rdbuf();
    fShaderFile.close();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char *vertexSource = vertexCode.c_str();
  const char *shaderSource = fragmentCode.c_str();

  unsigned int vertex = compileShader(GL_VERTEX_SHADER, &vertexSource);
  unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, &shaderSource);

  this->id = createShaderProgram(vertex, fragment);
}

int Shader::getId() const {
  return this->id;
}

void Shader::use() const {
  glUseProgram(this->id);
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(
      glGetUniformLocation(this->id, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const {
  glUniform3fv(
      glGetUniformLocation(this->id, name.c_str()), 1, value_ptr(value));
}

void Shader::setMatrix4(const std::string &name, mat4 value) const {
  glUniformMatrix4fv(
      glGetUniformLocation(this->id, name.c_str()),
      1,
      GL_FALSE,
      value_ptr(value));
}
