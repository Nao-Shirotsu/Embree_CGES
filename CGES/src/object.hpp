#pragma once

#include <cstdint>
#include <fstream>
#include <vector>
#include <string>

#include <glm/vec3.hpp>

#include <embree3/rtcore_geometry.h>
#include <embree3/rtcore_scene.h>

#include "util_objfile.hpp"

namespace cges {

// シーンに配置される任意の3Dモデルのクラス
class Object {
public:
  // とりあえず.obj(vとfのみ)からの読み込みだけ実装する
  inline Object(const RTCDevice device, const RTCScene scene)
      : m_geometry{ nullptr } {
    m_geometry = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
  }

  inline ~Object() {
    rtcReleaseGeometry(m_geometry);
  }

  inline void LoadDefault() {
    auto vertices = static_cast<float*>(rtcSetNewGeometryBuffer(m_geometry,
                                                                RTC_BUFFER_TYPE_VERTEX,
                                                                0,
                                                                RTC_FORMAT_FLOAT3,
                                                                3 * sizeof(float),
                                                                3));
    auto indices = static_cast<uint32_t*>(rtcSetNewGeometryBuffer(m_geometry,
                                                                  RTC_BUFFER_TYPE_INDEX,
                                                                  0,
                                                                  RTC_FORMAT_UINT3,
                                                                  3 * sizeof(uint32_t),
                                                                  1));
    if (vertices && indices) {
      vertices[0] = 0.f;
      vertices[1] = 0.f;
      vertices[2] = 0.f;
      vertices[3] = 1.f;
      vertices[4] = 0.f;
      vertices[5] = 0.f;
      vertices[6] = 0.f;
      vertices[7] = 1.f;
      vertices[8] = 0.f;
      indices[0] = 0;
      indices[1] = 1;
      indices[2] = 2;
    }
    rtcCommitGeometry(m_geometry);
  }

  // ロード成功でtrue, 失敗でfalseを返す
  inline bool LoadObjFile(const char* const objFileName) {
    auto ifs = std::ifstream(objFileName);
    if (!ifs) {
      return false;
    }
    struct PolygonVertices {
      float x, y, z;
    };

    struct PolygonIndices {
      uint32_t v0, v1, v2;
    };

    std::string marker;
    PolygonVertices vertexReceiver;
    PolygonIndices indexReceiver;
    std::vector<PolygonVertices> vertices;
    std::vector<PolygonIndices> indices;

    while (!ifs.eof()) {
      if (ifs.peek() == '#') {
        while (ifs.get() != '\n') {} // 1行飛ばす
        continue;
      }
      ifs >> marker;
      switch (obj::ToMarker(marker)) {
        case obj::Marker::V: {
          ifs >> vertexReceiver.x;
          ifs >> vertexReceiver.y;
          ifs >> vertexReceiver.z;
          vertices.push_back(vertexReceiver);
        } break;

        case obj::Marker::F: {
          ifs >> indexReceiver.v0;
          ifs >> indexReceiver.v1;
          ifs >> indexReceiver.v2;
          indices.push_back(indexReceiver);
        } break;

        default: {
          while (ifs.get() != '\n') {} // 1行飛ばす
          break;
        }
      }
    }

    auto verBuf = static_cast<float*>(rtcSetNewGeometryBuffer(m_geometry,
                                                              RTC_BUFFER_TYPE_VERTEX,
                                                              0,
                                                              RTC_FORMAT_FLOAT3,
                                                              sizeof(PolygonVertices),
                                                              vertices.size()));
    auto idxBuf = static_cast<uint32_t*>(rtcSetNewGeometryBuffer(m_geometry,
                                                                 RTC_BUFFER_TYPE_INDEX,
                                                                 0,
                                                                 RTC_FORMAT_UINT3,
                                                                 sizeof(PolygonIndices),
                                                                 indices.size()));
    if (verBuf && idxBuf) {
      std::memcpy(verBuf, &vertices[0], sizeof(PolygonVertices) * vertices.size());
      std::memcpy(idxBuf, &indices[0], sizeof(PolygonIndices) * indices.size());
      rtcCommitGeometry(m_geometry);
      return true;
    }

    return false;
  }


  inline uint32_t AttachTo(const RTCScene scene) {
    return rtcAttachGeometry(scene, m_geometry);
  }

private:
  //glm::vec3 origin; // モデル座標系原点
  RTCGeometry m_geometry;
};

} // namespace cges