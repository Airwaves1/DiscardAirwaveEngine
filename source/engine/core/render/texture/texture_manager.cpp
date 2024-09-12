// #include "texture_manager.hpp"
// #include "render/texture/texture_2d.hpp"
// #include "utils/log.hpp"
// namespace Airwave
// {
// std::shared_ptr<Texture2D> TextureManager::loadTexture2D(std::string name, const std::string &path,
//                                                          const TextureSpecification &spec)
// {
//     for (auto &item : m_loadTexture2DCache)
//     {
//         if (item.first == name)
//         {
//             return item.second;
//         }

//         if (item.second->getPath() == path)
//         {
//             return item.second;
//         }
//     }

//     auto texture               = std::make_shared<Texture2D>(path, spec);
//     m_loadTexture2DCache[name] = texture;

//     return texture;
// }

// std::shared_ptr<Texture2D> TextureManager::createTexture2D(std::string name, uint32_t width,
//                                                            uint32_t height,
//                                                            const TextureSpecification &spec)
// {
//     if (m_createdTexture2DCache.find(name) != m_createdTexture2DCache.end())
//     {
//         LOG_WARN("Texture with name {0} already exists!", name);
//         return m_createdTexture2DCache[name];
//     }

//     auto texture                  = std::make_shared<Texture2D>(width, height, spec);
//     m_createdTexture2DCache[name] = texture;

//     return texture;
// }

// std::shared_ptr<Texture2D> TextureManager::getLoadedTexture2D(const std::string &name)
// {
//     if (m_loadTexture2DCache.find(name) == m_loadTexture2DCache.end())
//     {
//         LOG_WARN("Texture with name {0} not found!", name);
//         return nullptr;
//     }

//     return m_loadTexture2DCache[name];
// }

// std::shared_ptr<Texture2D> TextureManager::getCreatedTexture2D(const std::string &name)
// {
//     if (m_createdTexture2DCache.find(name) == m_createdTexture2DCache.end())
//     {
//         LOG_WARN("Texture with name {0} not found!", name);
//         return nullptr;
//     }

//     return m_createdTexture2DCache[name];
// }

// } // namespace Airwave
