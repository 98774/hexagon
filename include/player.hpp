#include <glm/glm.hpp>

#define NUM_PLAYER_VERTICES 3
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

private:
  float m_radius;
  float m_angle;
  float vertices[NUM_PLAYER_VERTICES * 3] = {0.0f, 0.866025f, 0.0f, -0.5f, 0.0f,
                                             0.0f, 0.5f,      0.0f, 0.0f};
};
