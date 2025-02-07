
#include "vertex.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <vector>

std::vector<Vertex> generateHexagon(float radius, glm::u8vec4 color) {
  std::vector<Vertex> vertices;

  // Center vertex
  vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), color,
                        glm::vec2(0.5f, 0.5f));

  // Generate 6 outer vertices
  constexpr int SIDES = 6;
  const float angleStep = glm::radians(360.0f / SIDES);

  for (int i = 0; i <= SIDES; i++) { // Include last vertex to close shape
    float angle = i * angleStep;
    float x = radius * cos(angle);
    float y = radius * sin(angle);

    // Map hexagon vertices to a simple texture layout
    glm::vec2 texCoord =
        glm::vec2((x / (2 * radius)) + 0.5f, (y / (2 * radius)) + 0.5f);

    vertices.emplace_back(glm::vec3(x, y, 0.0f), color, texCoord);
  }

  return vertices;
}
