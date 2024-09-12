#pragma once

#include <string>

#ifdef ASSETS_DIR
#define SHADER_DIR ASSETS_DIR "shaders/"
#define TEXTURE_DIR ASSETS_DIR "textures/"
#define MODEL_DIR ASSETS_DIR "models/"
#endif

namespace Airwave
{
class FileUtils
{
  public:
    static std::string ReadFile(const std::string &filepath);

  private:
    FileUtils() = delete;
};
} // namespace Airwave
