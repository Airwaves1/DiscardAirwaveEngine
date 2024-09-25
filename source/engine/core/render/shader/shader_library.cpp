#include "shader_library.hpp"

namespace Airwave
{
    void ShaderLibrary::add(const std::string &name, const std::shared_ptr<Shader> &shader)
    {
        m_shaders[name] = shader;
    }

    void ShaderLibrary::add(const std::shared_ptr<Shader> &shader)
    {
        const std::string &name = shader->getName();
        add(name, shader);
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string &name,
                                                const std::string &vertex,
                                                const std::string &fragment,
                                                bool fromFile)
    {
        if(exists(name))
        {
            // LOG_WARN("Shader {0} already exists", name);
            return m_shaders[name];
        }
        
        auto shader = Shader::Create(vertex, fragment, fromFile);
        if(!shader)
        {
            LOG_ERROR("Failed to create shader {0}", name);
            return nullptr;
        }

        shader->setName(name);
        add(name, shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string &name, const std::string &filepath)
    {
        //TODO: Implement shader loading from single file
        return nullptr;
    }

    std::shared_ptr<Shader> ShaderLibrary::get(const std::string &name)
    {
        if(!exists(name))
        {
            LOG_WARN("Shader {0} not found", name);
            return nullptr;
        }

        return m_shaders[name];
    }


} // namespace Airwave
