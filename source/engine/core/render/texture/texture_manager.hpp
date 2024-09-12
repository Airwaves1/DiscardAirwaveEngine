// #pragma once

// #include "render/texture/texture.hpp"
// #include "render/texture/texture_2d.hpp"
// #include <unordered_map>

// namespace Airwave
// {

// class TextureManager
// {
//   public:
//     TextureManager() = default;

//     TextureManager(const TextureManager &)            = delete;
//     TextureManager &operator=(const TextureManager &) = delete;

//     // 从文件中加载2D纹理
//     std::shared_ptr<Texture2D>
//     loadTexture2D(std::string name, const std::string &path,
//                   const TextureSpecification &spec = TextureSpecification());
//     // 创建一个空的2D纹理
//     std::shared_ptr<Texture2D>
//     createTexture2D(std::string name, uint32_t width, uint32_t height,
//                     const TextureSpecification &spec = TextureSpecification());
//     // 根据名称获取纹理
//     std::shared_ptr<Texture2D> getLoadedTexture2D(const std::string &name);
//     std::shared_ptr<Texture2D> getCreatedTexture2D(const std::string &name);

//     void destroyTexture2D(const std::string &name)
//     {
//         m_loadTexture2DCache.erase(name);
//         m_createdTexture2DCache.erase(name);
//     }

//   private:
//     // 从文件中加载2D纹理的缓存
//     std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_loadTexture2DCache;

//     // 创建的2D纹理的缓存
//     std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_createdTexture2DCache;
// };

// } // namespace Airwave