#pragma once

#include "render_buffer.hpp"
#include "camera.hpp"

#include <memory>
#include <GLFW/glfw3.h>

namespace cges::gl {

// Graphics API�̃��b�p�[�N���X 
// �����E�B���h�E�͔�Ή�
class Engine {
public:
  Engine(const size_t windowWidth, const size_t windowHeight, const char* const windowTitle);
  ~Engine();

  void Update(Camera& camera);
  void Draw(const RenderBuffer& renderTarget) const;
  bool ShouldTerminate() const noexcept;

private:
  GLFWwindow* m_window;
  bool m_valid = false;
};

} // namespace gl::engine