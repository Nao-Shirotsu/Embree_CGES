#pragma once

#include <vector>
#include <memory>

#include <glm/vec3.hpp>
#include <embree3/rtcore.h>

#include "gameobject_base.hpp"
#include "light_source.hpp"

namespace cges {

// �V�[��
class Scene {
public:
  Scene(const RTCDevice rtcDevice);
  ~Scene() noexcept;

  void Update();
  void Add(std::unique_ptr<gameobject::Base> object);
  const RTCScene GetRTCScene() const;
  const glm::vec3& GetDirLightForward() const;
  const gameobject::Base& GetConstGameobject(const unsigned int geomID) const;

private:
  Scene(const Scene& other) = delete;
  Scene& operator=(const Scene& other) = delete;

  RTCScene m_rtcScene;
  std::vector<std::unique_ptr<gameobject::Base>> m_objects;
  std::vector<LightSource> m_lightSrcs;
  LightSource m_dirLight;
  bool sceneChanged;
};

} // namespace