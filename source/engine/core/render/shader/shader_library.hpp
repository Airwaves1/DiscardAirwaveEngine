#pragma once
#include "render/shader/shader.hpp"
#include "utils/common.hpp"
#include "utils/log.hpp"
#include <unordered_map>
#include <memory>

namespace Airwave
{

  #define SHADER_LIB ShaderLibrary::GetInstance() 
class ShaderLibrary
{
  public:
    static ShaderLibrary &GetInstance()
    {
        static ShaderLibrary instance;
        return instance;
    }

    ShaderLibrary() : m_shaders() {}
    ~ShaderLibrary() = default;

    void add(const std::string &name, const std::shared_ptr<Shader> &shader);
    void add(const std::shared_ptr<Shader> &shader);

    std::shared_ptr<Shader> load(const std::string &name, const std::string &vertex,
                                 const std::string &fragment, bool fromFile = true);

    std::shared_ptr<Shader> load(const std::string &name, const std::string &filepath);

    std::shared_ptr<Shader> get(const std::string &name);

    bool exists(const std::string &name) const { return m_shaders.find(name) != m_shaders.end(); }

  private:
    std::shared_ptr<Shader> m_tmpCreateShader;
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};
} // namespace Airwave
