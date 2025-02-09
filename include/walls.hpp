
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>
#include <vector>

#define NUM_WALL_ROWS 20
#define WALL_SEGMENTS 6  // Number of walls per loop (hexagon)
#define SPEED 1.0f       // Rate at which walls descend
#define THICKNESS 0.1f   // Scaling factor for the trapezoid height
#define RADIUS 3.0f      // Initial radius of the walls
#define MIN_RADIUS 0.05f // The point at which the walls disappear
#define OBSTACLE_INTERVAL 0.5f

// Obstacle
#define MAX_PATTERN_LENGTH 10
#define MAX_OBSTACLES 15
struct Obstacle {
  int layers;                               // number of layers in the pattern
  glm::uint8_t pattern[MAX_PATTERN_LENGTH]; // the pattern data
};

struct Wall {
  float radius; // Distance from the center
  bool visible;
};

class Walls {
public:
  Walls() {
    m_numSides = WALL_SEGMENTS;
    setupMesh();
    initObstacles();
  }

  void update(float deltaTime) {
    if (obstacle_timer > 0.0f) {
      obstacle_timer -= deltaTime;
    } else {
      if (m_obstacleQueue.empty()) {
        Obstacle newOb = getRandomObstacle();
        addObstacleToQueue(newOb);
      }
      addWallRow(m_obstacleQueue.front());
      m_obstacleQueue.pop();
      obstacle_timer = OBSTACLE_INTERVAL;
    }

    // Wait appropriate time before next obstacle
    vertices.clear();
    indices.clear();

    unsigned int vertexOffset = 0;
    for (size_t row = 0; row < NUM_WALL_ROWS; ++row) {
      for (size_t i = 0; i < m_numSides; ++i) {
        Wall &wall = wallMatrix[row][i];
        // std::cout << wall.visible << std::endl;
        if (!wall.visible)
          continue;

        // Shrink walls inward
        if (wall.radius > MIN_RADIUS) {
          wall.radius -= SPEED * deltaTime;
        }
        if (wall.radius < MIN_RADIUS)
          wall.visible = false;

        // Define base vertical line and rotate
        float innerRadius = wall.radius + THICKNESS;
        float outerRadius = wall.radius;
        float angle = glm::radians(i * 360.0f / m_numSides);
        float deltaAngle = glm::radians(360.0f / m_numSides);
        float cosA = glm::cos(angle), sinA = glm::sin(angle);
        float cosB = glm::cos(angle + deltaAngle),
              sinB = glm::sin(angle + deltaAngle);

        glm::vec2 v1 = glm::vec2(cosA * innerRadius, sinA * innerRadius);
        glm::vec2 v2 = glm::vec2(cosA * outerRadius, sinA * outerRadius);
        glm::vec2 v3 = glm::vec2(cosB * innerRadius, sinB * innerRadius);
        glm::vec2 v4 = glm::vec2(cosB * outerRadius, sinB * outerRadius);

        vertices.insert(vertices.end(), {v1.x, v1.y, 0.0f, v3.x, v3.y, 0.0f,
                                         v2.x, v2.y, 0.0f, v4.x, v4.y, 0.0f});

        // Define indices for two triangles per wall section
        indices.insert(indices.end(),
                       {vertexOffset, vertexOffset + 1, vertexOffset + 2,
                        vertexOffset + 1, vertexOffset + 3, vertexOffset + 2});
        vertexOffset += 4;
      }
    }

    // Update buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_DYNAMIC_DRAW);
  }

  void addWallRow(uint8_t newRow) {
    for (int i = 0; i < m_numSides; ++i) {
      wallMatrix[m_currentWallIndex][i] =
          Wall{RADIUS, newRow & (0x1 << i) ? true : false};
      m_currentWallIndex++;
      m_currentWallIndex %= NUM_WALL_ROWS;
    }
  }

  void render(Shader &shader, glm::mat4 &projection, glm::mat4 &view) {
    shader.use();
    shader.setMat4("mvp", projection * view);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  int getMaxObstacles() { return MAX_OBSTACLES; }
  Obstacle getObstacle(int index) { return m_obstacles[index]; }
  Obstacle getRandomObstacle() {
    int index = random() % MAX_OBSTACLES;
    return m_obstacles[index];
  }

private:
  GLuint VAO, VBO, EBO;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  int m_numSides;
  Wall wallMatrix[NUM_WALL_ROWS][WALL_SEGMENTS] = {0};
  int m_currentWallIndex = 0;
  Obstacle m_obstacles[MAX_OBSTACLES];
  std::queue<uint8_t> m_obstacleQueue;
  float obstacle_timer = 0.0f;

  void setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);
  }
  void initObstacles() {
    m_obstacles[0] = {5, {0b111110, 0b011111, 0b001111, 0b000111, 0b000011}};
    m_obstacles[1] = {4, {0b101010, 0b010101, 0b101010, 0b010101}};
    m_obstacles[2] = {
        6, {0b111000, 0b000111, 0b111000, 0b000111, 0b111000, 0b000111}};
    m_obstacles[3] = {5, {0b100001, 0b010010, 0b001100, 0b010010, 0b100001}};
    m_obstacles[4] = {
        7,
        {0b111011, 0b011110, 0b001100, 0b000000, 0b001100, 0b011110, 0b110111}};
    m_obstacles[5] = {5, {0b110011, 0b001100, 0b110011, 0b001100, 0b110011}};
    m_obstacles[6] = {5, {0b111100, 0b011110, 0b001111, 0b000011, 0b000001}};
    m_obstacles[7] = {5, {0b100100, 0b010010, 0b001001, 0b010010, 0b100100}};
    m_obstacles[8] = {
        6, {0b111000, 0b011100, 0b001110, 0b000111, 0b000011, 0b000001}};
    m_obstacles[9] = {
        6, {0b000111, 0b001110, 0b011100, 0b111000, 0b110000, 0b100000}};
    m_obstacles[10] = {
        6, {0b101101, 0b010010, 0b101101, 0b010010, 0b101101, 0b010010}};
    m_obstacles[11] = {5, {0b111011, 0b110111, 0b101111, 0b011111, 0b001111}};
    m_obstacles[12] = {5, {0b110111, 0b101101, 0b111011, 0b000000, 0b110011}};
    m_obstacles[13] = {
        6, {0b100001, 0b010010, 0b100001, 0b010010, 0b010010, 0b010010}};
    m_obstacles[14] = {
        6, {0b100001, 0b010010, 0b100001, 0b010010, 0b010010, 0b010010}};
  };
  void addObstacleToQueue(Obstacle &obstacle) {
    for (int i = 0; i < obstacle.layers; ++i) {
      m_obstacleQueue.push(obstacle.pattern[i]);
    }
  }
};
