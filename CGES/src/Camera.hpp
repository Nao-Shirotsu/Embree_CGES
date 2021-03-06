#pragma once

#include <glm/vec3.hpp>

namespace cges {

class Camera {
public:
  Camera(const glm::vec3& posWorld, const float radius, const float fov);

  void UpdatePosLocal();
  void StayPosition();
  glm::vec3 GetPosLocal() const;
  bool JustMoved() const;

  glm::vec3 posWorld = { 0.0f, 0.0f, 0.0f };
  glm::vec3 upwardWorld = { 0.0f, 1.0f, 0.0f };
  float radius = 5.0f; // 原点との距離
  float radXZ = 1.0f;  // x成分とz成分の間のなす角
  float radYZ = 1.0f;  // y成分とz成分の間のなす角
  float fov;           // 視野角

private:
  glm::vec3 m_posLocal;
  bool m_justMoved;
};

}// namespace cges