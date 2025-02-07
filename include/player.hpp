#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>

#define NUM_PLAYER_VERTICES 3
#define PLAYER_SCALE 0.15
class Player {

public:
  Player(float angle, float radius) {
    m_angle = angle;
    m_radius = radius;
  }
  void rotate(float angle) {}
  void render() {}
  float getAngle() { return m_angle; }
  float getRadius() { return m_radius; }
  void setAngle(float angle) { m_angle = angle; }
  void setRadius(float radius) { m_radius = radius; }
  int getNumVertices() { return NUM_PLAYER_VERTICES; }
  float *getVertices() { return vertices; }
  glm::mat4 getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f); // initialize to identity matrix
    model = glm::rotate(model, m_angle, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::translate(model, glm::vec3(-PLAYER_SCALE / 2, m_radius, 0.0f));
    model = glm::scale(model, glm::vec3(PLAYER_SCALE, PLAYER_SCALE, 1.0f));

    return model;
  }

private:
  float m_radius;
  float m_angle;
  float vertices[NUM_PLAYER_VERTICES * 3] = {0.5f, 0.866025f, 0.0f, 0.0f, 0.0f,
                                             0.0f, 1.0f,      0.0f, 0.0f};
};
