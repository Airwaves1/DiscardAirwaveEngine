#include "gl_cube_texture.hpp"
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <array>
#include "utils/common.hpp"
#include "utils/file_utils.hpp"
#include "utils/log.hpp"

namespace Airwave
{
OpenGLCubeTexture::OpenGLCubeTexture(const std::array<std::string, 6>& faces) {
    loadTexutres(faces);
}

OpenGLCubeTexture::~OpenGLCubeTexture() {
    glDeleteTextures(1, &m_rendererID);
}

void OpenGLCubeTexture::bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);
}

void OpenGLCubeTexture::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
void OpenGLCubeTexture::loadTexutres(const std::array<std::string, 6> &faces)
{
    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_rendererID);

    // 翻转
    stbi_set_flip_vertically_on_load(false);

    int width, height, channels;
    for (size_t i = 0; i < faces.size(); ++i)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (!data)
        {
            AW_ASSERT(false, "Failed to load cubemap texture at path: " + faces[i]);
            continue;
        }

        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
} // namespace Airwave
