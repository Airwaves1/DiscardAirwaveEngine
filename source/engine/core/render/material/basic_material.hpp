#pragma once
#include "material.hpp"
#include "utils/file_utils.hpp"
#include "render/shader/shader_library.hpp"
#include "render/texture/texture_2d.hpp"

namespace Airwave
{
class BasicMaterial : public Material
{
  public:
    BasicMaterial()
    {
        // m_shader = SHADER_LIB.load("basic_material", SHADER_DIR "shader_lib/basic.vert",
        //                            SHADER_DIR "shader_lib/basic.frag");
    }

    ~BasicMaterial() = default;

    virtual void bind() override
    {
        m_shader->bind();
        setUniform("u_textureCount", static_cast<int>(m_textures.size()));
    }
};
} // namespace Airwave