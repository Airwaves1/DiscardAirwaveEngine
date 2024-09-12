#include "shader_library.hpp"

namespace Airwave
{
    void ShaderLibrary::Add(const std::string &name, const std::shared_ptr<Shader> &shader)
    {
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const std::shared_ptr<Shader> &shader)
    {
        const std::string &name = shader->GetName();
        Add(name, shader);
    }

    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string &name,
                                                const std::string &vertex,
                                                const std::string &fragment,
                                                bool fromFile)
    {
        if(Exists(name))
        {
            LOG_WARN("Shader {0} already exists", name);
            return m_Shaders[name];
        }
        
        Shader* shader = Shader::Create(vertex, fragment, fromFile);
        if(!shader)
        {
            LOG_ERROR("Failed to create shader {0}", name);
            return nullptr;
        }

        auto sharedShader = std::shared_ptr<Shader>(shader);
        Add(name, sharedShader);
        return sharedShader;
    }

    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath)
    {
        //TODO: Implement shader loading from single file
        return nullptr;
    }

    std::shared_ptr<Shader> ShaderLibrary::Get(const std::string &name)
    {
        if(!Exists(name))
        {
            LOG_WARN("Shader {0} not found", name);
            return nullptr;
        }

        return m_Shaders[name];
    }


} // namespace Airwave
