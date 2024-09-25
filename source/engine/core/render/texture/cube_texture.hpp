#pragma once

#include "texture.hpp"
#include "texture_2d.hpp"

namespace Airwave
{
class CubeTexture : public Texture
{
  public:
    static std::shared_ptr<CubeTexture> Create(const std::string &path,
                                               const TextureSpecification &spec = TextureSpecification());
    static std::shared_ptr<CubeTexture> Create(uint32_t width, uint32_t height,
                                               const TextureSpecification &spec = TextureSpecification());
};

} // namespace Airwave
