
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
  Walls() { setupMesh(); }

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

  void render(Shader &shader) {
    shader.use();
    glBindVertexArray(VAO);

    for (size_t row = 0; row < wallMatrix.size(); ++row) {
      for (size_t i = 0; i < WALL_SEGMENTS; ++i) {
        if (queueMatrix[row][i] == 1) { // If wall exists
          Wall &wall = wallMatrix[row][i];

          glm::mat4 model = glm::mat4(1.0f);
          //          model = glm::translate(
          //              model,
          //              glm::vec3(wall.radius * cos(i * glm::radians(60.0f)),
          //                        wall.radius * sin(i * glm::radians(60.0f)),
          //                        0.0f));
          //          model = glm::rotate(model, glm::radians(i * 60.0f),
          //                              glm::vec3(0.0f, 0.0f, 1.0f));
          //          model = glm::scale(model, glm::vec3(1.0f,
          //          THICKNESS, 1.0f));
          std::cout << "model: " << model[0][0] << std::endl;
          shader.setMat4("model", model);
          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
  }

private:
  GLuint VAO, VBO, EBO;
  std::vector<std::vector<int>> queueMatrix; // Wall presence matrix
  std::vector<std::vector<Wall>> wallMatrix; // Actual wall data

  void setupMesh() {
    float trapezoidVertices[] = {
        // X, Y, Z
        -0.5f,  0.5f,  0.0f, // Top-left
        0.5f,   0.5f,  0.0f, // Top-right
        -0.25f, -0.5f, 0.0f, // Bottom-left
        0.25f,  -0.5f, 0.0f  // Bottom-right
    };

    unsigned int indices[] = {
        0, 1, 2, // First triangle
        1, 3, 2  // Second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trapezoidVertices), trapezoidVertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
};
