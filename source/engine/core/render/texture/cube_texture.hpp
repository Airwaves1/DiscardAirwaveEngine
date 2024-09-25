#pragma once

#include "texture.hpp"
#include "texture_2d.hpp"

namespace Airwave
{
class CubeTexture : public Texture
{
  public:
    static std::shared_ptr<CubeTexture> Create(const std::array<std::string, 6>& faces);
  
};

} // namespace Airwave
