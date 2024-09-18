#pragma once
#include <glm/glm.hpp>
#include "render/material/material.hpp"
#include "render/material/basic_material.hpp"
#include "render/texture/texture_2d.hpp"

namespace Airwave
{
struct BasicMaterialComponent
{
    BasicMaterialComponent() {}

    glm::vec4 color = glm::vec4(1.0f);
    std::shared_ptr<Material> material;
};

class PhongMaterialComponent
{
  public:
    PhongMaterialComponent()
    {
        material = std::make_shared<BasicMaterial>();
        material->setShader(SHADER_LIB.load("phong_material", SHADER_DIR "shader_lib/phong.vert",
                                            SHADER_DIR "shader_lib/phong.frag"));
    }

    glm::vec4 color    = glm::vec4(1.0f);
    glm::vec3 ambient  = glm::vec3(0.1f);
    glm::vec3 diffuse  = glm::vec3(0.5f);
    glm::vec3 specular = glm::vec3(1.0f);
    float shininess    = 32.0f;
    std::shared_ptr<Material> material;
};

} // namespace Airwave
