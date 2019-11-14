#pragma once

#include <embree3/rtcore_geometry.h>
#include <embree3/rtcore_scene.h>

namespace cges::gameobject{

// �V�[���ɔz�u�����C�ӂ�3D���f���̃N���X
class Base {
public:
  Base(const RTCDevice device, const RTCGeometryType geomType);

  virtual ~Base() noexcept;

  // ��K�{����(Embree�̃o�b�t�@��slot�Ɋi�[����l)�������Ă��邩�ǂ���
  virtual bool HasAttribute() = 0;

  // Embree�Ǘ����̃V�[���ɂ��̃I�u�W�F�N�g��o�^
  unsigned int AttachTo(const RTCScene scene);

protected:
  //glm::vec3 origin; // ���f�����W�n���_�̃��[���h�ʒu
  const RTCDevice m_rtcDevice;
  RTCGeometry m_rtcGeometry;
};

} // namespace cges::gameobject