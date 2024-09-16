#pragma once
#include "material.hpp"
#include "render/shader/shader_library.hpp"
#include "utils/file_utils.hpp"

namespace Airwave
{
class BasicMaterial : public Material
{
  public:
    BasicMaterial()
    {
        m_shader = SHADER_LIB.load("basic_material", SHADER_DIR "shader_lib/basic.vert",
                                   SHADER_DIR "shader_lib/basic.frag");
    }

    ~BasicMaterial() = default;

    virtual void bind() override { m_shader->bind(); }
};
} // namespace Airwave