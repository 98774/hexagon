#include "walls.hpp"

Walls::Walls() {
  m_numSides = WALL_SEGMENTS;
  setupMesh();
  initObstacles();
}

void Walls::update(float deltaTime) {
  updateObstacles(deltaTime);

  vertices.clear();
  indices.clear();
  unsigned int vertexOffset = 0;

  for (size_t row = 0; row < NUM_WALL_ROWS; ++row) {
    for (size_t i = 0; i < m_numSides; ++i) {
      Wall &wall = wallMatrix[row];
      wall.radius -= SPEED * deltaTime;
      if (wall.radius < MIN_RADIUS) {
        wall.radius = 0;
      }

      if (!(wall.pattern & (1 << i)))
        continue;

      float angle = glm::radians(i * 360.0f / m_numSides);
      float tempVertices[8];
      generateWallVertices(wall, angle, tempVertices);

      vertices.insert(vertices.end(),
                      {tempVertices[0], tempVertices[1], 0.0f, tempVertices[2],
                       tempVertices[3], 0.0f, tempVertices[4], tempVertices[5],
                       0.0f, tempVertices[6], tempVertices[7], 0.0f});

      indices.insert(indices.end(),
                     {vertexOffset, vertexOffset + 1, vertexOffset + 2,
                      vertexOffset + 1, vertexOffset + 2, vertexOffset + 3});

      vertexOffset += 4;
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);
}

bool Walls::checkPlayerCollision(glm::vec2 playerPos) {
  int numTriangles = indices.size() / 3;
  int inds[3];

  for (int i = 0; i < numTriangles; i++) {
    inds[0] = indices[i * 3];
    inds[1] = indices[i * 3 + 1];
    inds[2] = indices[i * 3 + 2];

    if (pointInTriangle(
            glm::vec2(vertices[inds[0] * 3], vertices[inds[0] * 3 + 1]),
            glm::vec2(vertices[inds[1] * 3], vertices[inds[1] * 3 + 1]),
            glm::vec2(vertices[inds[2] * 3], vertices[inds[2] * 3 + 1]),
            playerPos))
      return true;
  }
  return false;
}

void Walls::addWallRow(uint8_t newRow) {
  wallMatrix[m_currentWallIndex] = Wall{RADIUS, newRow};
  m_currentWallIndex = (m_currentWallIndex + 1) % NUM_WALL_ROWS;
}

void Walls::render(Shader &shader, glm::mat4 &projection, glm::mat4 &view) {
  shader.use();
  shader.setMat4("mvp", projection * view);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

int Walls::getMaxObstacles() { return MAX_OBSTACLES; }
Obstacle Walls::getObstacle(int index) { return m_obstacles[index]; }

Obstacle Walls::getRandomObstacle() {
  int index = rand() % MAX_OBSTACLES;
  return m_obstacles[index];
}

bool Walls::pointInTriangle(glm::vec2 C, glm::vec2 B, glm::vec2 A,
                            glm::vec2 P) {
  glm::vec2 v0 = C - A, v1 = B - A, v2 = P - A;
  float dot00 = glm::dot(v0, v0);
  float dot01 = glm::dot(v0, v1);
  float dot02 = glm::dot(v0, v2);
  float dot11 = glm::dot(v1, v1);
  float dot12 = glm::dot(v1, v2);
  float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
  float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
  return (u >= 0) && (v >= 0) && (u + v < 1);
}

void Walls::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBindVertexArray(0);
}

void Walls::initObstacles() {
  m_obstacles[0] = {5, {0b111110, 0b111101, 0b111011, 0b110111, 0b101111}};
  m_obstacles[1] = {5, {0b011111, 0b101111, 0b110111, 0b111011, 0b111101}};
  m_obstacles[2] = {
      6, {0b101010, 0b010101, 0b101010, 0b010101, 0b101010, 0b010101}};
  m_obstacles[3] = {
      6, {0b100100, 0b011011, 0b111000, 0b000111, 0b111000, 0b000111}};
  m_obstacles[4] = {5, {0b011111, 0b111011, 0b011111, 0b111011, 0b011111}};
  m_obstacles[5] = {
      6, {0b011011, 0b101101, 0b110110, 0b011011, 0b101101, 0b110110}};

  m_obstacles[6] = {5, {0b001111, 0b110011, 0b111100, 0b001111, 0b110011}};
  m_obstacles[7] = {5, {0b001111, 0b111001, 0b001111, 0b111001, 0b001111}};
};

void Walls::addObstacleToQueue(Obstacle &obstacle) {
  for (int i = 0; i < obstacle.layers; ++i) {
    m_obstacleQueue.push(obstacle.pattern[i]);
  }
}

void Walls::updateObstacles(float deltaTime) {
  // Start by updating the queue if it's low
  if (m_obstacleQueue.size() < 1) {
    Obstacle obstacle = getRandomObstacle();
    addObstacleToQueue(obstacle);
  }

  // Add wall if time has elapsed
  if (obstacle_timer < 0.0f) {
    addWallRow(m_obstacleQueue.front());
    m_obstacleQueue.pop();
    obstacle_timer = OBSTACLE_INTERVAL;
  }

  // Decrement timer
  obstacle_timer -= deltaTime;
}

void Walls::generateWallVertices(Wall &wall, float angle, float *vertices) {
  // Define base vertical line and rotate
  float innerRadius = wall.radius + THICKNESS;
  float outerRadius = wall.radius;
  float deltaAngle = glm::radians(360.0f / m_numSides);
  float cosA = glm::cos(angle), sinA = glm::sin(angle);
  float cosB = glm::cos(angle + deltaAngle),
        sinB = glm::sin(angle + deltaAngle);

  glm::vec2 v1 = glm::vec2(cosA * innerRadius, sinA * innerRadius);
  glm::vec2 v2 = glm::vec2(cosA * outerRadius, sinA * outerRadius);
  glm::vec2 v3 = glm::vec2(cosB * innerRadius, sinB * innerRadius);
  glm::vec2 v4 = glm::vec2(cosB * outerRadius, sinB * outerRadius);
  vertices[0] = v1.x;
  vertices[1] = v1.y;
  vertices[2] = v2.x;
  vertices[3] = v2.y;
  vertices[4] = v3.x;
  vertices[5] = v3.y;
  vertices[6] = v4.x;
  vertices[7] = v4.y;
}
