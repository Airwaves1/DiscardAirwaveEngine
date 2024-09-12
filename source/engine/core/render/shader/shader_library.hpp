#pragma once
#include "render/shader/shader.hpp"
#include "utils/common.hpp"
#include "utils/log.hpp"
#include <unordered_map>
#include <memory>

namespace Airwave
{
class ShaderLibrary
{
  public:
    ShaderLibrary() : m_Shaders() {}
    ~ShaderLibrary() = default;

    void Add(const std::string &name, const std::shared_ptr<Shader> &shader);
    void Add(const std::shared_ptr<Shader> &shader);

    std::shared_ptr<Shader> Load(const std::string &name, const std::string &vertex,
                                 const std::string &fragment, bool fromFile = true);

    std::shared_ptr<Shader> Load(const std::string &name, const std::string &filepath);

    std::shared_ptr<Shader> Get(const std::string &name);

    bool Exists(const std::string &name) const { return m_Shaders.find(name) != m_Shaders.end(); }

  private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
};
} // namespace Airwave
