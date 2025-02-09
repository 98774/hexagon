
#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "player.hpp"
#include "shader.hpp"
#include "walls.hpp"
#include <sys/types.h>
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

// Frame rate monitor
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Camera
Camera cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Initialize colors
glm::vec3 backgroundColor = glm::vec3(1.0f, 0.5f, 0.31f);
Player player = Player();

int main() {
  GLFWwindow *window = initializeGLFW();

  // build and compile our shader program
  // ------------------------------------
  Shader walls_shader("src/shaders/walls_vs.glsl", "src/shaders/walls_fs.glsl");

  Shader background_shader(
      "src/shaders/background_vs.glsl",
      "src/shaders/background_vs.glsl"); // you can name your shader
  Shader player_shader(
      "src/shaders/player_vs.glsl",
      "src/shaders/player_fs.glsl"); // you can name your shader

  glEnable(GL_DEPTH_TEST);

  // This line allows movement off the xz plane
  cam.Fpv = false;
  // VertexBuffer playerBuffer(player.getVertices(), player.getNumVertices());
  // Define perspective
  glm::mat4 projection = glm::mat4(1.0f);
  projection = glm::perspective(
      glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  glm::mat4 view = cam.GetViewMatrix();

  Walls wallMatrix = Walls();
  wallMatrix.addWallRow({1, 0, 1, 0, 1, 0});
  float wallCounter = 0;
  int sides = 6;
  float angleMod = 0;
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window, deltaTime);
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    wallCounter += deltaTime;
    if (wallCounter > 0.5) {
      wallCounter = 0;

      glm::uint8 vals = rand() % 64;
      wallMatrix.addWallRow({
          (vals & 0x1) ? 1 : 0, (vals & 0x2) ? 1 : 0, (vals & 0x4) ? 1 : 0,
          (vals & 0x8) ? 1 : 0, (vals & 0x10) ? 1 : 0,
          (vals & 0x20) ? 1 : 0 // Fixed incorrect mask from 0x2 to 0x20
      });
      //      sides = rand() % 6 + 3;
      //      wallMatrix.setNumSides(sides);
    }

    // render
    // ------
    glClearColor(0.1 + glm::sin(glfwGetTime() * M_PI) / 4,
                 0.1 + glm::sin(glfwGetTime() * M_PI) / 4,
                 0.1 + glm::sin(glfwGetTime() * M_PI) / 4, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to use the corresponding shader program first (to set the

    view = glm::rotate(view, (float)glm::radians(1.05), glm::vec3(0, 0, -1.0));

    player.render(player_shader, projection, view);

    wallMatrix.update(deltaTime);
    wallMatrix.render(walls_shader, projection, view);
    //    playerBuffer.render(player, projection, view);
    //    wallMatrix.update();
    //    wallMatrix.render(walls, projection, view);

    deltaTime = glfwGetTime() - lastFrame;
    lastFrame = glfwGetTime();

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
// fram need collision detection of obstacles with the cursor Other features and
// bugs

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float deltaTime) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) ||
      glfwGetKey(window, GLFW_KEY_CAPS_LOCK)) {
    glfwSetWindowShouldClose(window, 1);
  }
  if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) {
    float newAngle = player.getAngle() + deltaTime * M_PI * 500;
    player.setAngle(newAngle > 2 * M_PI ? 0 : newAngle);
  }
  if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) {
    float newAngle = player.getAngle() - deltaTime * M_PI * 500;
    player.setAngle(newAngle < 0 ? 2 * M_PI : newAngle);
  }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
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
