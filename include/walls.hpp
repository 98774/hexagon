#ifndef WALLS_HPP
#define WALLS_HPP

#include "shader.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>
#include <vector>

#define NUM_WALL_ROWS 20
#define WALL_SEGMENTS 6
#define SPEED 0.3f
#define THICKNESS 0.15f
#define RADIUS 3.0f
#define MIN_RADIUS 0.05f
#define OBSTACLE_INTERVAL 0.42f
#define MAX_PATTERN_LENGTH 10
#define MAX_OBSTACLES 8

struct Obstacle {
  int layers;
  glm::uint8_t pattern[MAX_PATTERN_LENGTH];
};

struct Wall {
  float radius;
  uint8_t pattern;
};

class Walls {
public:
  Walls();
  void update(float deltaTime);
  bool checkPlayerCollision(glm::vec2 playerPos);
  void addWallRow(uint8_t newRow);
  void render(Shader &shader, glm::mat4 &projection, glm::mat4 &view);

  int getMaxObstacles();
  Obstacle getObstacle(int index);
  Obstacle getRandomObstacle();

private:
  GLuint VAO, VBO, EBO;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  int m_numSides;
  Wall wallMatrix[NUM_WALL_ROWS] = {0};
  int m_currentWallIndex = 0;
  Obstacle m_obstacles[MAX_OBSTACLES];
  std::queue<uint8_t> m_obstacleQueue;
  float obstacle_timer = 0.0f;

  void setupMesh();
  void initObstacles();
  void addObstacleToQueue(Obstacle &obstacle);
  void updateObstacles(float deltaTime);
  bool pointInTriangle(glm::vec2 C, glm::vec2 B, glm::vec2 A, glm::vec2 P);
  void generateWallVertices(Wall &wall, float angle, float *vertices);
};

#endif
