
#include "GameStateManager.hpp"
#include <iostream>
// Example Menu State
//
class MenuState : public GameState {
  enum MenuItems { PLAY, OPTIONS, EXIT };

  MenuState() : currentItem(PLAY) {}

protected:
public:
  void onEnter() override { std::cout << "Entering MENU state\n"; }
  State update(GLFWwindow &window, float deltaTime) override {
    // Handle input, UI navigation
    return State::MENU;
  }
  void render() override {
    // Render menu screen
  }

  // Returns true if the game should continue running
  bool inputHandler(unsigned int keycode) override {

    if (keycode == GLFW_KEY_ESCAPE || keycode == GLFW_KEY_Q) {
      return false;
    }

    // Create looping menu
    if (keycode == GLFW_KEY_W || keycode == GLFW_KEY_UP) {
      currentItem--;
      if (currentItem < PLAY) {
        currentItem = EXIT;
      }
    }

    if (keycode == GLFW_KEY_S || keycode == GLFW_KEY_DOWN) {
      currentItem++;
      if (currentItem > EXIT) {
        currentItem = PLAY;
      }
    }

    if (keycode == GLFW_KEY_D || keycode == GLFW_KEY_RIGHT ||
        keycode == GLFW_KEY_ENTER) {
      selectCurrentItem();
    }
    return true;
  }

private:
  char *menuItems;
  int currentItem;

  void selectCurrentItem() {
    // Handle menu item selection
    switch (currentItem) {
    case PLAY:
      // Start game
      std::cout << "MenuState::selectCurrentItem::PLAY\n";
      break;
    case OPTIONS:
      // Show options
      std::cout << "MenuState::selectCurrentItem::OPTIONS\n";
      std::cout << "Selected Options\n";
      break;
    case EXIT:
      // Exit game
      std::cout << "MenuState::selectCurrentItem::EXIT\n";
      break;
    }
  }
};

// Example Playing State
class PlayingState : public GameState {
public:
  void onEnter() override { std::cout << "Entering PLAYING state\n"; }
  void update(float deltaTime) override {
    // Game logic
  }
  void render() override {
    // Render game objects
  }
};
