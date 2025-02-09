#include "glm/fwd.hpp"
#include <cstdlib>

#define MAX_PATTERN_LENGTH 10
#define MAX_OBSTACLES 15
struct Obstacle {
  int layers;                               // number of layers in the pattern
  glm::uint8_t pattern[MAX_PATTERN_LENGTH]; // the pattern data
};

class ObstacleList {
public:
  ObstacleList() {
    Obstacle obs[15] = {
        {5, {0b111110, 0b011111, 0b001111, 0b000111, 0b000011}},
        {4, {0b101010, 0b010101, 0b101010, 0b010101}},
        {6, {0b111000, 0b000111, 0b111000, 0b000111, 0b111000, 0b000111}},
        {5, {0b100001, 0b010010, 0b001100, 0b010010, 0b100001}},
        {7,
         {0b111111, 0b011110, 0b001100, 0b000000, 0b001100, 0b011110,
          0b111111}},
        {5, {0b110011, 0b001100, 0b110011, 0b001100, 0b110011}},
        {5, {0b111100, 0b011110, 0b001111, 0b000011, 0b000001}},
        {5, {0b100100, 0b010010, 0b001001, 0b010010, 0b100100}},
        {6, {0b111000, 0b011100, 0b001110, 0b000111, 0b000011, 0b000001}},
        {6, {0b000111, 0b001110, 0b011100, 0b111000, 0b110000, 0b100000}},
        {6, {0b101101, 0b010010, 0b101101, 0b010010, 0b101101, 0b010010}},
        {5, {0b111011, 0b110111, 0b101111, 0b011111, 0b001111}},
        {5, {0b111111, 0b000000, 0b111111, 0b000000, 0b111111}},
        {6, {0b100001, 0b010010, 0b100001, 0b010010, 0b010010, 0b010010}}};
  }

  int getMaxObstacles() { return MAX_OBSTACLES; }
  Obstacle getObstacle(int index) { return obstacles[index]; }
  Obstacle getRandomObstacle() {
    int index = random() % MAX_OBSTACLES;
    return obstacles[index];
  }

private:
  Obstacle obstacles[MAX_OBSTACLES];
};
