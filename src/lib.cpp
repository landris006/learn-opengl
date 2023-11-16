#include "glm/ext/matrix_clip_space.hpp"
#include <sys/types.h>
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
  glm::vec2 texCoord;
};

// Vertex vertices[] = {
//     {.position = {0.5f, 0.5f, 0.0f}, .texCoord = {1.0, 1.0}},
//     {.position = {0.5f, -0.5f, 0.0f}, .texCoord = {1.0, 0.0}},
//     {.position = {-0.5f, -0.5f, 0.0f}, .texCoord = {0.0, 0.0}},
//     {.position = {-0.5f, 0.5f, 0.0f}, .texCoord = {0.0, 1.0}}};
// unsigned int indices[] = {3, 2, 1, 3, 1, 0};
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.5f,
    0.5f,  -0.5f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  -0.5f, 0.5f,
    -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,
    1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  -0.5f,
    0.5f,  -0.5f, 1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f,
    -0.5f, 0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,
    1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  1.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  -0.5f,
    -0.5f, -0.5f, 0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  1.0f,  0.5f,  -0.5f,
    0.5f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,
    1.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  -0.5f,
    0.5f,  -0.5f, 0.0f,  1.0f};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};

void render(const Shader &shaderProgram) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      (const void *)(offsetof(Vertex, texCoord)));
  glEnableVertexAttribArray(1);

  unsigned int vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // unsigned int elementBufferObject;
  // glGenBuffers(1, &elementBufferObject);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  // glBufferData(
  //     GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  shaderProgram.use();
  shaderProgram.setInt("texture1", 0);
  shaderProgram.setInt("texture2", 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

  glm::mat4 view = glm::mat4(1.0f);
  // note that we’re translating the scene in the reverse direction
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection;
  projection =
      glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  shaderProgram.setMatrix4("view", view);
  shaderProgram.setMatrix4("projection", projection);

  for (uint i = 0; i < 10; i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i + (i % 3 == 0 ? glfwGetTime() * 10 : 0);
    model =
        glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    shaderProgram.setMatrix4("model", model);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

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

  glEnable(GL_DEPTH_TEST);

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
