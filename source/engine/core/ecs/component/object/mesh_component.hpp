#pragma once
#include "render/geometry/geometry.hpp"
#include "render/material/material.hpp"
#include "ecs/component/geometry/geometry_component.hpp"
#include "ecs/component/material/material_component.hpp"
#include "ecs/aw_component.hpp"

namespace Airwave
{
struct MeshComponent
{
    MeshComponent() = default;

    std::string name;
    std::shared_ptr<GeometryComponent> geometryComponent;
    std::shared_ptr<MaterialComponent> materialComponent;

    std::weak_ptr<MeshComponent> parent;
    std::vector<std::shared_ptr<MeshComponent>> children;

    void addChild(const std::shared_ptr<MeshComponent> &child)
    {
        if(child)
        {
            child->parent = std::make_shared<MeshComponent>(*this);
            children.push_back(child);
        }
    }

    std::shared_ptr<MeshComponent> getChild(const std::string &name)
    {
        for(auto &child : children)
        {
            if(child->name == name)
            {
                return child;
            }
        }
        return nullptr;
    }

    void removeChild(const std::shared_ptr<MeshComponent> &child)
    {
        if(child)
        {
            auto it = std::find(children.begin(), children.end(), child);
            if(it != children.end())
            {
                children.erase(it);
            }
        }
    }

    void clearChildren()
    {
        children.clear();
    }


};


} // namespace Airwave
