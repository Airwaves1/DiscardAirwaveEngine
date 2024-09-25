#pragma once

#include "system.hpp"
#include "render/material/material.hpp"
#include "render/shader/shader_library.hpp"
#include "ecs/component/material/material_component.hpp"
#include "ecs/component/camera/camera_component.hpp"
#include "ecs/component/transform/transform_component.hpp"
#include "ecs/component/light/light_component.hpp"
#include "render/texture/texture_2d.hpp"

namespace Airwave
{
class MaterialSystem : public System
{

  public:
    MaterialSystem() {}
    virtual ~MaterialSystem() {}

    virtual void update(std::shared_ptr<Scene> scene, float deltaTime) {}

    void updateMaterial(std::shared_ptr<MaterialComponent> &mat, CameraComponent &cameraComponent,
                        TransformComponent &transformComponent, float deltaTime)
    {
        switch (mat->type)
        {
            case MaterialType::BlinnPhong:
            {
                if (mat->shader == nullptr)
                {
                    auto shader = SHADER_LIB.load("blinn_phong", SHADER_DIR "shader_lib/blinn_phong.vert",
                                                  SHADER_DIR "shader_lib/blinn_phong.frag");
                    mat->shader = shader;
                }
                auto shader = mat->shader;
                shader->bind();
                shader->uploadUniformMat4("u_viewMatrix", cameraComponent.camera->getViewMatrix());
                shader->uploadUniformMat4("u_projectionMatrix", cameraComponent.camera->getProjectionMatrix());
                shader->uploadUniformMat4("u_modelMatrix", transformComponent.getTransformMatrix());
                shader->uploadUniformFloat3("u_cameraPosition", cameraComponent.camera->getPosition());
                shader->uploadUniformFloat3("u_material.color", mat->diffuse);
                shader->uploadUniformFloat3("u_material.ambient", mat->ambient);
                shader->uploadUniformFloat3("u_material.specular", mat->specular);
                shader->uploadUniformFloat("u_material.shininess", mat->shininess);
                shader->uploadUniformFloat("u_material.alpha", mat->alpha);
                shader->uploadUniformInt("u_material.diffuseMap", 0);
                shader->uploadUniformInt("u_material.specularMap", 1);

                break;
            }
            case MaterialType::Unlit:
            {
                break;
            }
            case MaterialType::Custom:
            {
                break;
            }
            case MaterialType::Standard:
            {
                break;
            }
        }

        if (mat->diffuseMap == nullptr)
        {
            static bool flag = false;
            if (!flag)
            {
                LOG_WARN("MaterialComponent: {0} has no diffuse map, use default diffuse map", mat->name);
                flag            = true;
                mat->diffuseMap = Texture2D::Create(TEXTURE_DIR "R-C.jpeg");
            }
            // LOG_WARN("MaterialComponent: {0} has no diffuse map, use default diffuse map", mat->name);
        }
        else
        {
            mat->diffuseMap->bind(0);
        }

        if (mat->specularMap == nullptr)
        {
            static bool flag = false;
            if (!flag)
            {
                mat->specularMap = Texture2D::Create(TEXTURE_DIR "R-C.jpeg");
                LOG_WARN("MaterialComponent: {0} has no specular map, use default specular map", mat->name);
                flag = true;
            }
            // LOG_WARN("MaterialComponent: {0} has no specular map, use default specular map", mat->name);
        }
        else
        {
            mat->specularMap->bind(1);
        }
    }

  private:
};
} // namespace Airwave
