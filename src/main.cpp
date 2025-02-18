
#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "player.hpp"
#include "shader.hpp"
#include "walls.hpp"
#include <GL/gl.h>
#include <sys/types.h>
#include <unistd.h>
#define BEFORE_GLFW
#include "GLFW/glfw3.h"
#include <cstdlib>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

GLFWwindow *initializeGLFW();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, float deltaTime);

enum State { MENU, PLAYING, PAUSED, GAME_OVER, EXIT };
int currentState = PLAYING;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float MIN_UPDATE_TIME = 0.01;
float elapsedTime = 0.0f;

// Frame rate monitor
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Camera
Camera cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::mat4 projection = glm::mat4(1.0f);

// Initialize colors
glm::vec3 backgroundColor = glm::vec3(1.0f, 0.5f, 0.31f);
Player player = Player();

int main() {
  srand(rand() ^ time(NULL));
  GLFWwindow *window = initializeGLFW();

  // build and compile our shader program
  // ------------------------------------
  Shader walls_shader("src/shaders/walls_vs.glsl", "src/shaders/walls_fs.glsl");

  Shader background_shader(
      "src/shaders/background_vs.glsl",
      "src/shaders/background_fs.glsl"); // you can name your shader
  Shader player_shader(
      "src/shaders/player_vs.glsl",
      "src/shaders/player_fs.glsl"); // you can name your shader

  // Put background canvas in the back
  glEnable(GL_DEPTH_TEST);

  float background_vertices[18] = {-1.0f, -1.0f, 0.0,  -1.0f, 1.0f,  0.0f,
                                   1.0f,  1.0f,  0.0f, -1.0f, -1.0f, 0.0f,
                                   1.0f,  -1.0f, 0.0f, 1.0f,  1.0f,  0.0f};
  unsigned int backgroundVAO, backgroundVBO;
  glGenVertexArrays(1, &backgroundVAO);
  glGenBuffers(1, &backgroundVBO);

  glBindVertexArray(backgroundVAO);

  glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(background_vertices),
               background_vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  // END BACKGROUND INIT

  // Define perspective
  projection = glm::perspective(
      glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = cam.GetViewMatrix();

  Walls wallMatrix = Walls();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  bool collision = false;
  std::cout << wallMatrix.PointInTriangle(glm::vec2(-1, 1), glm::vec2(-0.5, 1),
                                          glm::vec2(-0.5, 0.5),
                                          glm::vec2(-0.6, 0.76))
            << std::endl;
  while (!glfwWindowShouldClose(window)) {
    processInput(window, deltaTime);

    // Time update
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Collision check
    if (wallMatrix.checkPlayerCollision(player.getTipPoint())) {
      collision = true;
    }

    // Add to add rotation
    view = glm::rotate(
        view, (float)glm::radians(1.05 + collision * 0.5),
        glm::vec3(0.0f, !collision * -glm::abs(glm::sin(glfwGetTime()) * 0.3),
                  -1.0));

    // Only render when enough time passes
    if (elapsedTime > MIN_UPDATE_TIME) {
      // render
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Background logic
      background_shader.use();
      background_shader.setInt("numSides", WALL_SEGMENTS);
      background_shader.setFloat("time", glfwGetTime());
      background_shader.setMat4("view", projection * view);
      glBindVertexArray(backgroundVAO);
      //      glDrawArrays(GL_TRIANGLES, 0, 6);

      player.render(player_shader, projection, view);
      wallMatrix.render(walls_shader, projection, view);

      elapsedTime = 0;
    }
    elapsedTime += deltaTime;

    if (!collision) {
      wallMatrix.update(deltaTime);
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse
    // moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// Overall plan:
// hexagon in the center of the screen
// create a rotating triangle that spins around the hexagon with the arror/ ad
// keys walls should come into the screen that align with the hexagon really
// need a queue of obstacles defined by a set of vertices that can slide into
// fram need collision detection of obstacles with the cursor Other features
// and bugs

// process all input: query GLFW whether relevant keys are pressed/released
// this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) ||
      glfwGetKey(window, GLFW_KEY_CAPS_LOCK)) {
    glfwSetWindowShouldClose(window, 1);
  }
  if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) {
    float newAngle = player.getAngle() + deltaTime * 8;
    player.setAngle(newAngle > 2 * M_PI ? 0 : newAngle);
  }
  if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) {
    float newAngle = player.getAngle() - deltaTime * 8;
    player.setAngle(newAngle < 0 ? 2 * M_PI : newAngle);
  }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina
  // displays.
  glViewport(0, 0, width, height);
  projection = glm::perspective(glm::radians(45.0f),
                                (float)width / (float)height, 0.1f, 100.0f);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {}

GLFWwindow *initializeGLFW() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "hexagon", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }

  return window;
}
