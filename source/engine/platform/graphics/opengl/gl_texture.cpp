#include "gl_texture.hpp"

#include <stb_image/stb_image.h>

#include "utils/common.hpp"
#include "utils/file_utils.hpp"
#include "utils/log.hpp"

namespace Airwave
{

OpenGLTexture2D::OpenGLTexture2D(const std::string &path, const TextureSpecification &spec)
{

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    AW_ASSERT(data, "Failed to load image!:"+path);

    m_width         = width;
    m_height        = height;
    m_specification = spec;
    m_path          = path;

    if (channels == 3)
    {
        m_specification.format         = TextureFormat::RGB;
        m_specification.internalFormat = TextureInternalFormat::RGB8;
    }
    else if (channels == 4)
    {
        m_specification.format         = TextureFormat::RGBA;
        m_specification.internalFormat = TextureInternalFormat::RGBA8;
    }
    else
    {
        AW_ASSERT(false, "Unsupported image format!");
    }

    GLenum internalFormat = TextureInternalFormatToGL(m_specification.internalFormat);
    GLenum dataFormat     = TextureFormatToGL(m_specification.format);
    GLenum wrapS          = TextureWrapModeToGL(m_specification.wrapS);
    GLenum wrapT          = TextureWrapModeToGL(m_specification.wrapT);
    GLenum minFilter      = TextureFilterModeToGL(m_specification.minFilter);
    GLenum magFilter      = TextureFilterModeToGL(m_specification.magFilter);

    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, dataFormat,
                 GL_UNSIGNED_BYTE, data);

    if (m_specification.generateMipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const TextureSpecification &spec)
{
    m_width         = width;
    m_height        = height;
    m_specification = spec;

    GLenum internalFormat = TextureInternalFormatToGL(m_specification.internalFormat);
    GLenum dataFormat     = TextureFormatToGL(m_specification.format);
    GLenum wrapS          = TextureWrapModeToGL(m_specification.wrapS);
    GLenum wrapT          = TextureWrapModeToGL(m_specification.wrapT);
    GLenum minFilter      = TextureFilterModeToGL(m_specification.minFilter);
    GLenum magFilter      = TextureFilterModeToGL(m_specification.magFilter);

    if (m_specification.enableMSAA)
    {
        glGenTextures(1, &m_rendererID);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_rendererID);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_specification.samples, internalFormat,
                                m_width, m_height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else
    {
        glGenTextures(1, &m_rendererID);
        glBindTexture(GL_TEXTURE_2D, m_rendererID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, dataFormat,
                     GL_UNSIGNED_BYTE, nullptr);

        if (m_specification.generateMipmaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_rendererID); }

void OpenGLTexture2D::bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Airwave::OpenGLTexture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

void Airwave::OpenGLTexture2D::setData(void *data, uint32_t size)
{
    AW_ASSERT(data, "Data pointer is null!");

    // 如果启用了多重采样，无法手动设置纹理数据
    if (m_specification.enableMSAA)
    {
        AW_ASSERT(false, "Cannot set data on multisample textures!");
        return;
    }

    GLenum dataFormat     = TextureFormatToGL(m_specification.format);
    GLenum internalFormat = TextureInternalFormatToGL(m_specification.internalFormat);

    // 计算预期的数据大小（以字节为单位），确保传入的数据大小与纹理规格匹配
    uint32_t expectedSize = m_width * m_height * GetChannelCount(dataFormat);
    AW_ASSERT(!(size == expectedSize), "Data size does not match texture size!");

    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    // 上传完整的纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, dataFormat,
                 GL_UNSIGNED_BYTE, data);

    if (m_specification.generateMipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<Texture> OpenGLTexture2D::clone()
{
    // 创建具有相同规格的新纹理对象
    auto clonedTexture = std::make_shared<OpenGLTexture2D>(m_width, m_height, m_specification);

    if (!m_path.empty())
    {
        clonedTexture->m_path = m_path; // 保留路径信息
    }

    // 如果是多重采样纹理，不能复制数据，只能克隆规格
    if (m_specification.enableMSAA)
    {
        // 多重采样纹理无法手动设置数据，因此无需执行 setData 操作
        return clonedTexture;
    }

    // 对于非多重采样纹理，复制当前纹理的数据到新的纹理对象
    auto textureData = getTextureData();
    clonedTexture->setData(textureData.data(), textureData.size());

    return clonedTexture;
}

GLenum OpenGLTexture2D::TextureWrapModeToGL(TextureWrapMode mode)
{
    switch (mode)
    {
        case TextureWrapMode::REPEAT:
            return GL_REPEAT;
        case TextureWrapMode::MIRRORED_REPEAT:
            return GL_MIRRORED_REPEAT;
        case TextureWrapMode::CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
        case TextureWrapMode::CLAMP_TO_BORDER:
            return GL_CLAMP_TO_BORDER;
        default:
            AW_ASSERT(false, "Unknown TextureWrapMode!");
            return 0;
    }
}

GLenum OpenGLTexture2D::TextureFilterModeToGL(TextureFilterMode mode)
{
    switch (mode)
    {
        case TextureFilterMode::NEAREST:
            return GL_NEAREST;
        case TextureFilterMode::LINEAR:
            return GL_LINEAR;
        default:
            AW_ASSERT(false, "Unknown TextureFilterMode!");
            return 0;
    }
}

GLenum OpenGLTexture2D::TextureFormatToGL(TextureFormat format)
{
    switch (format)
    {
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        case TextureFormat::DEPTH:
            return GL_DEPTH_COMPONENT;
        default:
            AW_ASSERT(false, "Unknown TextureFormat!");
            return 0;
    }
}

GLenum OpenGLTexture2D::TextureInternalFormatToGL(TextureInternalFormat format)
{
    switch (format)
    {
        case TextureInternalFormat::RGB8:
            return GL_RGB8;
        case TextureInternalFormat::RGBA8:
            return GL_RGBA8;
        case TextureInternalFormat::RGB16F:
            return GL_RGB16F;
        case TextureInternalFormat::RGBA16F:
            return GL_RGBA16F;
        case TextureInternalFormat::DEPTH24:
            return GL_DEPTH_COMPONENT24;
        case TextureInternalFormat::DEPTH32F:
            return GL_DEPTH_COMPONENT32F;
        case TextureInternalFormat::DEPTH24STENCIL8:
            return GL_DEPTH24_STENCIL8;
        default:
            AW_ASSERT(false, "Unknown TextureInternalFormat!");
            return 0;
    }
}

uint32_t OpenGLTexture2D::GetChannelCount(GLenum format)
{
    switch (format)
    {
        case GL_RGB:
            return 3;
        case GL_RGBA:
            return 4;
        case GL_RED:
            return 1;
        default:
            return 0;
    }
}

std::vector<unsigned char> OpenGLTexture2D::getTextureData()
{
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    GLenum format = TextureFormatToGL(m_specification.format);
    GLenum type   = GL_UNSIGNED_BYTE; // 假设数据是 8 位格式

    size_t dataSize = m_width * m_height * GetChannelCount(format);
    std::vector<unsigned char> data(dataSize);

    glGetTexImage(GL_TEXTURE_2D, 0, format, type, data.data());

    glBindTexture(GL_TEXTURE_2D, 0);

    return data;
}

} // namespace Airwave
