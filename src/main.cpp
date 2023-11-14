#include <filesystem>
#include <format>
#include <iostream>
#include <math.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <shader.h>

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// clang-format off
  float vertices[] = {
     0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0.0f
  };
  unsigned int indices[] = {
    3, 2, 0,
    // 1, 2, 3
  };
// clang-format on

void render(Shader &shaderProgram) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  unsigned int vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);
  // clang-format off
  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    reinterpret_cast<void *>(0));
  // clang-format on
  glEnableVertexAttribArray(0);

  // clang-format off
  glVertexAttribPointer(
    1,
    3,
    GL_FLOAT,
    GL_FALSE,
    6 * sizeof(float),
    reinterpret_cast<void *>(3* sizeof(float)));
  // clang-format on
  glEnableVertexAttribArray(1);

  unsigned int vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int elementBufferObject;
  glGenBuffers(1, &elementBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  shaderProgram.use();
  float offSet = sin(glfwGetTime()) / 2.0f;
  shaderProgram.setFloat("offsetX", offSet);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

int main(int argc, char *argv[]) {
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

  // glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(
      window, [](GLFWwindow *window, int width, int height) -> void {
        glViewport(0, 0, width, height);
      });

  Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    render(shaderProgram);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
