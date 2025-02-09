
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>

#define NUM_PLAYER_VERTICES 3
#define PLAYER_SCALE 0.10
#define PLAYER_RADIUS 0.2

class Player {
public:
  Player() {
    m_angle = 0.0f;
    m_radius = PLAYER_RADIUS;
    m_VAO = 0;
    m_VBO = 0;
  }

  void render(Shader &shader, glm::mat4 &projection, glm::mat4 &view) {
    if (!m_VAO || !m_VBO) {
      initializeGL();
    }

    glm::mat4 mvp = projection * view * getModelMatrix();
    shader.use();
    shader.setMat4("mvp", mvp);
    shader.setMat4("model", getModelMatrix());

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, NUM_PLAYER_VERTICES);
    glBindVertexArray(0);
  }

  // Used to defer initialization of GL functions until after GL context exists
  bool initializeGL() {
    if (glGenVertexArrays == nullptr) {
      std::cerr << "OpenGL function glGenVertexArrays is NULL! OpenGL is not "
                   "initialized.\n";
      return false;
    }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
                          (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return true;
  }
  float getAngle() { return m_angle; }
  float getRadius() { return m_radius; }
  void setAngle(float angle) { m_angle = angle; }
  void setRadius(float radius) { m_radius = radius; }
  glm::mat4 getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, m_angle, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::translate(model, glm::vec3(0.0f, m_radius, 0.0f));
    model = glm::scale(model, glm::vec3(PLAYER_SCALE, PLAYER_SCALE, 1.0f));
    return model;
  }

private:
  float m_radius;
  float m_angle;
  unsigned int m_VAO, m_VBO;

  // Fix array size (NUM_PLAYER_m_vertices * 3)
  float m_vertices[NUM_PLAYER_VERTICES * 3] = {
      0.0f,   0.5f,   0.0f, // Top
      -0.43f, -0.25f, 0.0f, // Bottom-left
      0.43f,  -0.25f, 0.0f  // Bottom-right
  };
};
