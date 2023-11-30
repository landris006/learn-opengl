#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include <camera.h>
#include <cmath>
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

using glm::vec2;
using glm::vec3;

static float VIEWPORT_WIDTH = 1600.0f;
static float VIEWPORT_HEIGHT = 800.0f;

static Camera camera = Camera();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;

float lastX = 400, lastY = 300;

struct Vertex {
  vec3 position;
  vec3 normal;
  vec2 texCoord;
};

struct Material {
  vec3 albedo;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

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

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
    -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
    -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
    0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
    -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
    -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

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
  glVertexAttribPointer(
      2,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(Vertex),
      (const void *)(offsetof(Vertex, texCoord)));
  glEnableVertexAttribArray(2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, vec3(3.0f, -3.0f, 3.0f));
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projection = camera.getProjectionMatrix();

  float lightX = 0.0f;
  float lightY = 2.0f;
  vec3 lightPos = vec3(lightX, lightY, 0.0f);

  shaderProgram.use();
  shaderProgram.setMatrix4("model", model);
  shaderProgram.setMatrix4("view", view);
  shaderProgram.setMatrix4("projection", projection);

  shaderProgram.setInt("material.diffuseMap", 0);
  shaderProgram.setInt("material.specularMap", 1);
  shaderProgram.setFloat("material.shininess", 32.0f);

  shaderProgram.setVec3("light.ambient", vec3(0.2f));
  shaderProgram.setVec3("light.diffuse", vec3(0.5f));
  shaderProgram.setVec3("light.specular", vec3(1.0f));

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

  glm::vec3 lightColor = glm::vec3(1.0f);

  model = glm::mat4(1.0f);
  model = glm::translate(model, lightPos);
  model = glm::scale(model, vec3(0.2f));
  lightShaderProgram.use();
  lightShaderProgram.setMatrix4("model", model);
  lightShaderProgram.setMatrix4("view", view);
  lightShaderProgram.setMatrix4("projection", projection);
  lightShaderProgram.setVec3("lightColor", lightColor);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);

  // std::cout << "render took " << std::round(deltaTime * 100) / 100 << " ms"
  //           << "(" << std::round(1 / deltaTime) << " fps)" << std::endl;
}

unsigned int loadTexture(const char *path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

  if (!data) {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
    return textureID;
  }

  GLenum format;
  if (nrComponents == 1)
    format = GL_RED;
  else if (nrComponents == 3)
    format = GL_RGB;
  else if (nrComponents == 4)
    format = GL_RGBA;

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      format,
      width,
      height,
      0,
      format,
      GL_UNSIGNED_BYTE,
      data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  return textureID;
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

  glActiveTexture(GL_TEXTURE0);
  loadTexture("assets/images/container2.png");
  glActiveTexture(GL_TEXTURE1);
  loadTexture("assets/images/container2_specular.png");

  camera.translation = vec3(0.0f, 0.0f, 3.0f);
  camera.vfov = 45.0f;

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
