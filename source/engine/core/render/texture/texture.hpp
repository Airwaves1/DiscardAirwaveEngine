#pragma once

#include "utils/common.hpp"
#include <memory>
#include <string>

namespace Airwave
{
enum class TextureWrapMode
{
    REPEAT          = 0,
    MIRRORED_REPEAT = 1,
    CLAMP_TO_EDGE   = 2,
    CLAMP_TO_BORDER = 3
};

enum class TextureFilterMode
{
    NEAREST = 0,
    LINEAR  = 1
};

enum class TextureFormat
{
    RGB   = 0,
    RGBA  = 1,
    DEPTH = 2
};

enum class TextureInternalFormat
{
    RGB8            = 0,
    RGBA8           = 1,
    RGB16F          = 2,
    RGBA16F         = 3,
    DEPTH24         = 4,
    DEPTH32F        = 5,
    DEPTH24STENCIL8 = 6,
    SRGB8           = 7,
    SRGBA8          = 8
};

struct TextureSpecification
{
    TextureWrapMode wrapS       = TextureWrapMode::REPEAT;
    TextureWrapMode wrapT       = TextureWrapMode::REPEAT;
    TextureFilterMode minFilter = TextureFilterMode::LINEAR;
    TextureFilterMode magFilter = TextureFilterMode::LINEAR;

    TextureFormat format                 = TextureFormat::RGBA;
    TextureInternalFormat internalFormat = TextureInternalFormat::RGBA8;

    bool generateMipmaps = false;
    bool enableMSAA      = false;
    uint16_t samples     = 1;

    TextureSpecification() = default;
};

class Texture
{
  public:
    Texture()          = default;
    virtual ~Texture() = default;

    // 禁止拷贝
    Texture(const Texture &)            = delete;
    Texture &operator=(const Texture &) = delete;

    virtual std::shared_ptr<Texture> clone() { return nullptr; }
    virtual uint32_t getWidth() const { return m_width; }
    virtual uint32_t getHeight() const { return m_height; }

    virtual void resize(uint32_t width, uint32_t height)
    {
        m_width  = width;
        m_height = height;
    }

    virtual void bind(uint32_t slot = 0) const = 0;
    virtual void unbind() const                = 0;

    virtual void setData(void *data, uint32_t size) {}
    virtual uint32_t getRendererID() const = 0;

    virtual const std::string &getPath() const { return m_path; }

    virtual const TextureSpecification &getSpecification() const { return m_specification; }

  protected:
    uint32_t m_width  = 0;
    uint32_t m_height = 0;
    std::string m_path{};
    TextureSpecification m_specification;
};

} // namespace Airwave