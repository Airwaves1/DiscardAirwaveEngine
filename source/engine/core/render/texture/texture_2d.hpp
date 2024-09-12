#pragma once

#include "render/texture/texture.hpp"


namespace Airwave
{

class Texture2D : public Texture
{
  public:
    // 从文件中加载
    static std::shared_ptr<Texture2D>
    Create(const std::string &path, const TextureSpecification &spec = TextureSpecification());
    // 创建一个空的纹理
    static std::shared_ptr<Texture2D>
    Create(uint32_t width, uint32_t height,
           const TextureSpecification &spec = TextureSpecification());
};
} // namespace  Airwave
