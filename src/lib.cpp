#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include <camera.h>
#include <cstdio>
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
#include <transform.h>

using glm::vec3;

static float VIEWPORT_WIDTH = 1600.0f;
static float VIEWPORT_HEIGHT = 800.0f;

static Camera camera = Camera();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;

float lastX = 400, lastY = 300;

void processInput(GLFWwindow *window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const float cameraSpeed = 3.0 * deltaTime;

  vec3 front = camera.front();
  vec3 right = camera.right();

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.translation += cameraSpeed * front;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.translation -= cameraSpeed * front;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.translation -= cameraSpeed * right;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.translation += cameraSpeed * right;
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    camera.translation += cameraSpeed * vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera.translation -= cameraSpeed * vec3(0.0f, 1.0f, 0.0f);
  }
}

struct Vertex {
  vec3 position;
  vec3 normal;
  glm::vec2 texCoord;
};

float vertices[] = {
    // positions // normals // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  -0.5f,
    0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, 0.0f,
    0.0f,  -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
    1.0f,  1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,
    1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  -0.5f,
    -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f,
    0.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,
    0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,
    1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,
    -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,
    0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,
    0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,
    0.0f,  1.0f};

void render(const Shader &shaderProgram, const Shader &lightShaderProgram) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
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
      (const void *)(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, vec3(3.0f, -3.0f, 3.0f));
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projection = camera.getProjectionMatrix();

  float lightX = 1.0f + sin(glfwGetTime()) * 2.0f;
  float lightY = sin(glfwGetTime() / 2.0f) * 1.0f;
  vec3 lightPos = vec3(lightX, lightY, 2.0f);

  shaderProgram.use();
  shaderProgram.setMatrix4("model", model);
  shaderProgram.setMatrix4("view", view);
  shaderProgram.setMatrix4("projection", projection);
  shaderProgram.setVec3("objectColor", vec3(1.0f, 0.5f, 0.31f));
  shaderProgram.setVec3("lightColor", vec3(1.0f, 1.0f, 1.0f));
  shaderProgram.setVec3("lightPos", lightPos);
  shaderProgram.setVec3("viewPos", camera.translation);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      (const void *)(offsetof(Vertex, position)));
  glEnableVertexAttribArray(0);

  model = glm::mat4(1.0f);
  model = glm::translate(model, lightPos);
  model = glm::scale(model, vec3(0.2f));
  lightShaderProgram.use();
  lightShaderProgram.setMatrix4("model", model);
  lightShaderProgram.setMatrix4("view", view);
  lightShaderProgram.setMatrix4("projection", projection);

  glDrawArrays(GL_TRIANGLES, 0, 36);

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

  GLFWwindow *window = glfwCreateWindow(
      VIEWPORT_WIDTH, VIEWPORT_HEIGHT, "LearnOpenGL", NULL, NULL);
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

  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glEnable(GL_DEPTH_TEST);

  loadTextures();
  loadTextures();

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow *window, int width, int height) -> void {
        glViewport(0, 0, width, height);
      });

  glfwSetCursorPosCallback(
      window, [](GLFWwindow *window, double xpos, double ypos) -> void {
        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;

        if (firstMouse) {
          lastX = xpos;
          lastY = ypos;
          firstMouse = false;
          return;
        }

        lastX = xpos;
        lastY = ypos;
        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        float newPitch = camera.pitch() - yoffset;
        float newYaw = camera.yaw() + xoffset;

        if (newPitch < -89.0f)
          newPitch = -89.0f;
        if (newPitch > 89.0f)
          newPitch = 89.0f;

        camera.setEuler(newPitch, newYaw, 0.0f);
      });

  glfwSetScrollCallback(
      window, [](GLFWwindow *window, double xoffset, double yoffset) -> void {
        float newVfov = camera.vfov - yoffset;
        if (newVfov < 1.0f)
          newVfov = 1.0f;
        if (newVfov > 45.0f)
          newVfov = 45.0f;

        camera.vfov = newVfov;
      });

  Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
  Shader lightShaderProgram(
      "shaders/vertex.glsl", "shaders/light.fragment.glsl");

  camera.translation = vec3(0.0f, 0.0f, 3.0f);
  camera.vfov = 45.0f;
  // camera.lookAt(vec3(0.0f, 0.0f, 2.0f));

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window, deltaTime);

    render(shaderProgram, lightShaderProgram);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
