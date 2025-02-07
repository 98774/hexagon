
#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glad/glad.h>
#define GLAD
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct Vertex {
  glm::vec3 position; // (x, y, z)
  glm::u8vec4 color;  // (r, g, b, a) as unsigned bytes (0-255)
  glm::vec2 texCoord; // (u, v)

  Vertex(glm::vec3 pos, glm::u8vec4 col, glm::vec2 tex)
      : position(pos), color(col), texCoord(tex) {};
};

class VertexBuffer {
public:
  GLuint VAO, VBO;

  // Pass in an array of vertices of the form Vertex
  // TODO
  // Add flags that we OR together and include the various attribute params for
  VertexBuffer(float *vertices, int count) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, (void *)vertices,
                 GL_STATIC_DRAW);

    // Position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  VertexBuffer(const Vertex *vertices, int count) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * count, (void *)vertices,
                 GL_STATIC_DRAW);

    // Position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Color (location = 1), using GL_UNSIGNED_BYTE and normalized to [0,1]
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                          (void *)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // Texture Coordinates (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void bind() const { glBindVertexArray(VAO); }

  void unbind() const { glBindVertexArray(0); }

  ~VertexBuffer() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
  }
};

#endif // VERTEX_HPP
