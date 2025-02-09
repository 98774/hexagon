
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define WALL_SEGMENTS 6 // Number of walls per loop (hexagon)
#define SPEED 0.01f     // Rate at which walls descend
#define THICKNESS 0.2f  // Scaling factor for the trapezoid height

struct Wall {
  float radius; // Distance from the center
};

class Walls {
public:
  Walls() {
    getWallVertices(6, 1.0f);
    setupMesh();
  }

  void update() {
    // Move walls inward
    for (auto &row : wallMatrix) {
      for (auto &wall : row) {
        if (wall.radius >
            0.1f) { // Keep a minimum radius to prevent disappearing instantly
          wall.radius -= SPEED;
        }
      }
    }
  }

  void render(Shader &shader, glm::mat4 &projection, glm::mat4 &view) {
    shader.use();
    glBindVertexArray(VAO);

    for (size_t row = 0; row < wallMatrix.size(); ++row) {
      for (size_t i = 0; i < WALL_SEGMENTS; ++i) {
        if (queueMatrix[row][i] == 1) { // If wall exists
          Wall &wall = wallMatrix[row][i];

          glm::mat4 model = glm::mat4(1.0f);
          model = glm::rotate(model, glm::radians(i * 60.0f),
                              glm::vec3(0.0f, 0.0f, -1.0f));

          model = glm::scale(model, glm::vec3(wall.radius, wall.radius, 1.0f));
          //          std::cout << "model: " << model[0][0] << std::endl;
          shader.setMat4("model", model);
          shader.setMat4("mvp", projection * view * model);
          glDrawArrays(GL_TRIANGLES, 0, 6);
        }
      }
    }

    glBindVertexArray(0);
  }

  void addWallRow(const std::vector<int> &newRow) {
    std::vector<Wall> newWallRow;
    for (int i = 0; i < WALL_SEGMENTS; ++i) {
      if (newRow[i] == 1) {
        newWallRow.push_back(Wall{1.0f}); // Start walls at radius = 1.0
      } else {
        newWallRow.push_back(Wall{-1.0f}); // -1.0 means it won't render
      }
    }
    queueMatrix.push_back(newRow);
    wallMatrix.push_back(newWallRow);
    std::cout << "Added wall row" << std::endl;
  }

private:
  GLuint VAO, VBO, EBO;
  std::vector<std::vector<int>> queueMatrix; // Wall presence matrix
  std::vector<std::vector<Wall>> wallMatrix; // Actual wall data
  float m_vertices[18];

  void setupMesh() {
    //    float trapezoidVertices[] = {
    //        // X, Y, Z
    //        -0.5f,  0.5f,  0.0f, // Top-left
    //        0.5f,   0.5f,  0.0f, // Top-right
    //        -0.25f, -0.5f, 0.0f, // Bottom-left
    //        0.25f,  -0.5f, 0.0f  // Bottom-right
    //    };
    //
    //    unsigned int indices[] = {
    //        0, 1, 2, // First triangle
    //        1, 3, 2  // Second triangle
    //    };

    std::cout << "Setting up mesh" << std::endl;
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

  void getWallVertices(int numSides, float radius) {

    float angleStep = glm::radians(360.0f / numSides);

    float innerRadius = radius;
    float outerRadius = radius + THICKNESS;
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
