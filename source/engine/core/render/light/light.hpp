#pragma once
#include <glm/glm.hpp>

namespace Airwave
{
enum class LightType
{
    Directional,
    Point,
    Spot
};

class Light
{
  public:
    virtual void applyLighting() = 0;
    LightType getType() { return type; }

  protected:
    LightType type;
};

class DirectionalLight : public Light
{
  public:
    DirectionalLight() { type = LightType::Directional; }

    glm::vec3 direction;
    glm::vec3 color;
    glm::vec3 intensity;

    virtual void applyLighting() override {}
};

class PointLight : public Light
{
  public:
    PointLight() { type = LightType::Point; }

    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 intensity;

    float constant;  // 衰减常数项
    float linear;    // 衰减线性项
    float quadratic; // 衰减二次项

    virtual void applyLighting() override {}
};

class SpotLight : public Light
{
  public:
    SpotLight() { type = LightType::Spot; }

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    glm::vec3 intensity;

    float constant;  // 衰减常数项
    float linear;    // 衰减线性项
    float quadratic; // 衰减二次项

    float cutOff;      // 光锥内角
    float outerCutOff; // 光锥外角

    virtual void applyLighting() override {}
};

} // namespace  Airwave
