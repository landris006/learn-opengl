#define GLFW_INCLUDE_NONE
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/trigonometric.hpp"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <format>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <shader.h>
#include <stb_image.h>

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 texCoords;
};

Vertex vertices[] = {
    {.position = {0.5f, 0.5f, 0.0f},
     .color = {1.0f, 0.0f, 0.0f},
     .texCoords = {1.0, 1.0}},
    {.position = {0.5f, -0.5f, 0.0f},
     .color = {0.0f, 1.0f, 0.0f},
     .texCoords = {1.0, 0.0}},
    {.position = {-0.5f, -0.5f, 0.0f},
     .color = {0.0f, 0.0f, 1.0f},
     .texCoords = {0.0, 0.0}},
    {.position = {-0.5f, 0.5f, 0.0f},
     .color = {1.0f, 1.0f, 0.0f},
     .texCoords = {0.0, 1.0}}};
unsigned int indices[] = {3, 2, 1, 3, 1, 0};

void render(const Shader &shaderProgram) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  unsigned int vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);
  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      (const void *)(offsetof(Vertex, position)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      (const void *)(offsetof(Vertex, color)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      (const void *)(offsetof(Vertex, texCoords)));
  glEnableVertexAttribArray(2);

  unsigned int vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int elementBufferObject;
  glGenBuffers(1, &elementBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  shaderProgram.use();
  shaderProgram.setInt("texture1", 0);
  shaderProgram.setInt("texture2", 1);

  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(
      transform,
      glm::vec3(
          sin(glfwGetTime() + 3.14 / 2) / 2, sin(glfwGetTime()) / 2, 0.0));
  transform = glm::rotate(
      transform, static_cast<float>(glfwGetTime()), glm::vec3(0.0, 0.0, 1.0));
  transform = glm::scale(
      transform, static_cast<float>(sin(glfwGetTime())) * glm::vec3(1.0));
  // transform =
  //     glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0, 1.0, 0.0));
  // transform =
  //     glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0, 0.0, 0.0));

  shaderProgram.setMatrix4("transform", transform);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void loadTextures() {
  stbi_set_flip_vertically_on_load(true);

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("assets/images/container.jpg", &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    return;
  }

  unsigned int texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      width,
      height,
      0,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  data = stbi_load(
      "assets/images/awesomeface.png", &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Failed to load texture" << std::endl;
    stbi_image_free(data);
    return;
  }

  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      width,
      height,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}

int run() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow *window, int width, int height) -> void {
        glViewport(0, 0, width, height);
      });

  Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  loadTextures();
  loadTextures();

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    render(shaderProgram);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
