
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define WALL_SEGMENTS 6 // Number of walls per loop (hexagon)
#define SPEED 1.0f      // Rate at which walls descend
#define THICKNESS 0.5f  // Scaling factor for the trapezoid height
#define RADIUS 2.0f     // Initial radius of the walls

struct Wall {
  float radius; // Distance from the center
  bool visible;
};

class Walls {
public:
  Walls() {
    m_numSides = WALL_SEGMENTS;
    getWallVertices();
    setupMesh();
  }

  void update(float deltaTime) {
    // Move walls inward
    for (auto &row : wallMatrix) {
      for (auto &wall : row) {
        if (wall.radius >
            0.05f) { // Keep a minimum radius to prevent disappearing instantly
          wall.radius -= SPEED * deltaTime;
        } else {
          wall.visible = 0;
        }
      }
    }
  }

  void render(Shader &shader, glm::mat4 &projection, glm::mat4 &view) {
    shader.use();
    glBindVertexArray(VAO);

    for (size_t row = 0; row < wallMatrix.size(); ++row) {
      for (size_t i = 0; i < WALL_SEGMENTS; ++i) {
        if (wallMatrix[row][i].visible == 1) { // If wall exists
          Wall &wall = wallMatrix[row][i];

          glm::mat4 model = glm::mat4(1.0f);
          model = glm::scale(model, glm::vec3(wall.radius, wall.radius, 1.0f));
          model = glm::rotate(model, glm::radians(i * 360.0f / m_numSides),
                              glm::vec3(0.0f, 0.0f, -1.0f));

          //          std::cout << "model: " << model[0][0] << std::endl;
          shader.setMat4("model", model);
          shader.setMat4("mvp", projection * view * model);
          glDrawArrays(GL_TRIANGLES, 0, 6);
        }
      }
    }

    glBindVertexArray(0);
  }

  void setNumSides(int sides) {
    m_numSides = sides;
    getWallVertices();
    setupMesh();
  }
  void addWallRow(const std::vector<int> &newRow) {
    std::vector<Wall> newWallRow;
    for (int i = 0; i < WALL_SEGMENTS; ++i) {
      if (newRow[i] == 1) {
        newWallRow.push_back(Wall{RADIUS, true});
      } else {
        newWallRow.push_back(Wall{RADIUS, false});
      }
    }
    wallMatrix.push_back(newWallRow);
  }

private:
  GLuint VAO, VBO;
  std::vector<std::vector<Wall>> wallMatrix; // Actual wall data
  float m_vertices[18];
  int m_numSides;

  void setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void getWallVertices() {

    float angleStep = glm::radians(360.0f / m_numSides);

    float innerRadius = RADIUS;
    float outerRadius = RADIUS + THICKNESS;
    // Compute vertex positions
    float x1_outer = outerRadius * cos(0);
    float y1_outer = outerRadius * sin(0);
    float x2_outer = outerRadius * cos(angleStep);
    float y2_outer = outerRadius * sin(angleStep);

    float x1_inner = innerRadius * cos(0);
    float y1_inner = innerRadius * sin(0);
    float x2_inner = innerRadius * cos(angleStep);
    float y2_inner = innerRadius * sin(angleStep);

    // Allocate memory for vertices (6 vertices * 3 coordinates each)
    float vertices[18] = {// First triangle
                          x1_outer, y1_outer, 0.0f, x2_outer, y2_outer, 0.0f,
                          x1_inner, y1_inner, 0.0f,

                          // Second triangle
                          x2_outer, y2_outer, 0.0f, x2_inner, y2_inner, 0.0f,
                          x1_inner, y1_inner, 0.0f};

    for (int i = 0; i < 18; i++) {
      std::cout << vertices[i] << std::endl;
      m_vertices[i] = vertices[i];
    }
  }
};
