#include "gl_shader.hpp"

#include "utils/common.hpp"
#include "utils/log.hpp"
#include "utils/file_utils.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Airwave
{

static uint32_t StringToGLShaderType(const std::string &type)
{
    if (type == "vertex") return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
    if (type == "compute") return GL_COMPUTE_SHADER;
    if (type == "geometry") return GL_GEOMETRY_SHADER;
    LOG_ERROR("Unknown shader type!");
    return -1;
}

OpenGLShader::OpenGLShader(const std::string &vertex, const std::string &fragment, bool fromFile)
{
    std::string vertexSrc;
    std::string fragmentSrc;
    if (fromFile)
    {
        vertexSrc   = FileUtils::ReadFile(vertex);
        fragmentSrc = FileUtils::ReadFile(fragment);
    }
    else
    {
        vertexSrc   = vertex;
        fragmentSrc = fragment;
    }

    m_openGLSourceCode[ShaderType::Vertex]   = vertexSrc;
    m_openGLSourceCode[ShaderType::Fragment] = fragmentSrc;

    compile();
}

OpenGLShader::~OpenGLShader() { glDeleteProgram(m_rendererID); }

void OpenGLShader::compile()
{
    if(m_rendererID != -1)
    {
        glDeleteProgram(m_rendererID);
    }

    GLuint vertexShader  = glCreateShader(GL_VERTEX_SHADER);
    const GLchar *source = (const char *)m_openGLSourceCode[ShaderType::Vertex].c_str();
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(vertexShader);

        LOG_ERROR("Vertex shader compilation failed: {0}", infoLog.data());

        return;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    source                = (const char *)m_openGLSourceCode[ShaderType::Fragment].c_str();
    glShaderSource(fragmentShader, 1, &source, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(fragmentShader);

        LOG_ERROR("Fragment shader compilation failed: {0}", infoLog.data());

        return;
    }

    m_rendererID = glCreateProgram();

    glAttachShader(m_rendererID, vertexShader);
    glAttachShader(m_rendererID, fragmentShader);

    glLinkProgram(m_rendererID);

    GLint isLinked = 0;
    glGetProgramiv(m_rendererID, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(m_rendererID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        LOG_ERROR("Shader link failed: {0}", infoLog.data());

        return;
    }

    glDetachShader(m_rendererID, vertexShader);
    glDetachShader(m_rendererID, fragmentShader);
}

void OpenGLShader::addMacro(const std::string macroNanme, const std::string &macroValue)
{
    std::string define = "#define " + macroNanme + " " + macroValue + "\n";
    m_openGLSourceCode[ShaderType::Vertex] = define + m_openGLSourceCode[ShaderType::Vertex];
    m_openGLSourceCode[ShaderType::Fragment] = define + m_openGLSourceCode[ShaderType::Fragment];
    compile();
}

void OpenGLShader::bind() const { glUseProgram(m_rendererID); }

void OpenGLShader::unbind() const { glUseProgram(0); }

void OpenGLShader::compileOrGetVulkanBinaries(
    const std::unordered_map<ShaderType, std::string> &shaderSources)
{
    // TODO: Compile the shader source code
}

void OpenGLShader::compileOrGetOpenGLBinaries() {}

void OpenGLShader::reflect(ShaderType stage, const std::vector<uint32_t> &shaderData) {}

void OpenGLShader::uploadUniformInt(const std::string &name, int value)
{
    GLint location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
            LOG_ERROR("Uniform {0} not found!", name);
            m_uniformErrordCache[name] = true;
        }
        return;
    }

    glUniform1i(location, value);
}

void OpenGLShader::uploadUniformFloat(const std::string &name, float value)
{
    GLint location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
            LOG_ERROR("Uniform {0} not found!", name);
            m_uniformErrordCache[name] = true;
        }
        return;
    }
    glUniform1f(location, value);
}

void OpenGLShader::uploadUniformFloat2(const std::string &name, const glm::vec2 &vector2)
{
    GLint location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
            LOG_ERROR("Uniform {0} not found!", name);
            m_uniformErrordCache[name] = true;
        }
        return;
    }
    glUniform2f(location, vector2.x, vector2.y);
}

void OpenGLShader::uploadUniformFloat3(const std::string &name, const glm::vec3 &vector3)
{
    GLint location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
            LOG_ERROR("Uniform {0} not found!", name);
            m_uniformErrordCache[name] = true;
        }
        return;
    }
    glUniform3f(location, vector3.x, vector3.y, vector3.z);
}

void OpenGLShader::uploadUniformFloat4(const std::string &name, const glm::vec4 &vector4)
{
    GLint location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
            LOG_ERROR("Uniform {0} not found!", name);
            m_uniformErrordCache[name] = true;
        }
        return;
    }
    glUniform4f(location, vector4.x, vector4.y, vector4.z, vector4.w);
}

void OpenGLShader::uploadUniformMat3(const std::string &name, const glm::mat3 &matrix)
{
    GLint location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
            LOG_ERROR("Uniform {0} not found!", name);
            m_uniformErrordCache[name] = true;
        }
        return;
    }
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::uploadUniformMat4(const std::string &name, const glm::mat4 &matrix)
{
    GLint location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
            LOG_ERROR("Uniform {0} not found!", name);
            m_uniformErrordCache[name] = true;
        }
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

} // namespace Airwave
