#include "Camera.hpp"

#include <cmath>

cges::Camera::Camera(const glm::vec3& posWorld, const float radius) 
  : posWorld{ posWorld }
  , radius{ radius } {
}

void cges::Camera::UpdatePosLocal() {
  const float sinYZ = std::sinf(radYZ);
  posLocal.x = radius * sinYZ * std::sinf(radXZ);
  posLocal.y = radius * std::cosf(radYZ);
  posLocal.z = radius * sinYZ * std::cosf(radXZ);
}

glm::vec3 cges::Camera::GetPosLocal() const{
  return posLocal;
}
