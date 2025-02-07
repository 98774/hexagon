
#ifndef GAME_STATE_MANAGER_HPP
#define GAME_STATE_MANAGER_HPP

#include <GLFW/glfw3.h>
#include <chrono>
#include <memory>
#include <unordered_map>

// Abstract base class for game states
class GameState {

public:
  enum State { MENU, PLAYING, PAUSED, GAME_OVER, EXIT };
  virtual ~GameState() = default;
  virtual void onEnter() {}                  // Called when entering the state
  virtual void onExit() {}                   // Called when leaving the state
  virtual State update(float deltaTime) = 0; // Game logic update
  virtual void render() = 0;                 // Rendering
  virtual bool inputHandler(unsigned int keycode) = 0; // Handle input
};

// Manages game states and handles transitions
// class GameStateManager {
//
// public:
//
// private:
//  GLFWwindow *window;
//  State currentState;
//  std::unordered_map<State, std::unique_ptr<GameState>> states;
//  bool running;
//  int targetFPS;
//  std::chrono::steady_clock::time_point lastFrameTime;
//
// public:
//  GameStateManager(GLFWwindow *win, int fps = 60)
//      : window(win), running(true), targetFPS(fps), currentState(State::MENU)
//      {
//    lastFrameTime = std::chrono::steady_clock::now();
//  }
//
//  void changeState(State newState) {
//    states[currentState]->onExit();
//    currentState = newState;
//    states[currentState]->onEnter();
//  }
//
//  void run() {
//    while (running && !glfwWindowShouldClose(window)) {
//      auto now = std::chrono::steady_clock::now();
//      float deltaTime =
//          std::chrono::duration<float>(now - lastFrameTime).count();
//      lastFrameTime = now;
//
//      // Update and render current state
//      states[currentState]->update(deltaTime);
//      render();
//
//      // TODO: add frame rate control
//      //  Frame rate control
//      //       float frameTime = 1.0f / targetFPS;
//      //      std::this_thread::sleep_for(
//      //         std::chrono::duration<float>(frameTime - deltaTime));
//
//      glfwSwapBuffers(window);
//      glfwPollEvents();
//    }
//  }
//
//  void stop() { running = false; }
//
//  void render() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    states[currentState]->render();
//  }
//};

#endif // GAME_STATE_MANAGER_HPP
