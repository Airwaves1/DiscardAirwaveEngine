#pragma once

#include "render/geometry/geometry.hpp"
#include "render/material/material.hpp"

namespace Airwave
{
class Mesh
{
  public:
    Mesh(const std::shared_ptr<Geometry> &geo, const std::shared_ptr<Material> &mat)
    {
        if (!geo || !mat)
        {
            LOG_ERROR("Geometry and Material must not be null");
            return;
        }

        m_geometry = geo;
        m_material = mat;
    }

    void setGeometry(const std::shared_ptr<Geometry> &geo)
    {
        if (!geo)
        {
            LOG_ERROR("Geometry must not be null");
            return;
        }
        m_geometry = geo;
    }

    void setMaterial(const std::shared_ptr<Material> &mat)
    {
        if (!mat)
        {
            LOG_ERROR("Material must not be null");
            return;
        }
        m_material = mat;
    }

    const std::shared_ptr<Geometry> &getGeometry() const { return m_geometry; }
    const std::shared_ptr<Material> &getMaterial() const { return m_material; }

  private:
    std::shared_ptr<Geometry> m_geometry;
    std::shared_ptr<Material> m_material;
};

} // namespace Airwave
