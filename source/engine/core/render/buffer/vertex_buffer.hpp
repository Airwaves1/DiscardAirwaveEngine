#pragma once

#include "utils/common.hpp"
#include "utils/log.hpp"

namespace Airwave
{

enum class ShaderDataType
{
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    INT,
    INT2,
    INT3,
    INT4,
    MAT3,
    MAT4,
    BOOL
};

// 正常的函数在多个cpp里会出现重定义，但是这里有static就不一样了，每一个函数都是该cpp的namespace范围里适用的
static uint32_t GetShaderDataTypeSize(const ShaderDataType &type)
{
    switch (type)
    {
        // 32位机和64位机上int和float都是4个字节, 只有long和
        // 指针是4和8个字节的区别
        case ShaderDataType::FLOAT:
            return 4;
        case ShaderDataType::FLOAT2:
            return 4 * 2;
        case ShaderDataType::FLOAT3:
            return 4 * 3;
        case ShaderDataType::FLOAT4:
            return 4 * 4;
        case ShaderDataType::INT:
            return 4;
        case ShaderDataType::INT2:
            return 4 * 2;
        case ShaderDataType::INT3:
            return 4 * 3;
        case ShaderDataType::INT4:
            return 4 * 4;
        case ShaderDataType::MAT3:
            return 4 * 3 * 3;
        case ShaderDataType::MAT4:
            return 4 * 4 * 4;
        case ShaderDataType::BOOL:
            return 1;
    }

    AW_ASSERT(false, "Unknown ShaderDataType")
    return -1;
}

// 主要是用于glVertexAttribPointer函数, 用于描述数据里的GL_FLOAT或者
// GL_INT的个数
static uint32_t GetShaderTypeDataCount(const ShaderDataType &type)
{
    switch (type)
    {
        case ShaderDataType::FLOAT:
            return 1;
        case ShaderDataType::FLOAT2:
            return 2;
        case ShaderDataType::FLOAT3:
            return 3;
        case ShaderDataType::FLOAT4:
            return 4;
        case ShaderDataType::INT:
            return 1;
        case ShaderDataType::INT2:
            return 2;
        case ShaderDataType::INT3:
            return 3;
        case ShaderDataType::INT4:
            return 4;
        case ShaderDataType::MAT3:
            return 3 * 3;
        case ShaderDataType::MAT4:
            return 4 * 4;
        case ShaderDataType::BOOL:
            return 1;
    }
    AW_ASSERT(false, "Unknown Shader Data Type");
    return -1;
}

class BufferElement
{
  public:
    BufferElement(const ShaderDataType &type, const std::string &name, bool isNormalized = false)
        : m_type(type), m_name(name), m_size(GetShaderDataTypeSize(type)), m_offset(0),
          m_isNormalized(isNormalized)
    {
    }

    void SetOffset(uint32_t offset) { m_offset = offset; }

    uint32_t getSize() const { return m_size; }
    uint32_t getOffset() const { return m_offset; }
    ShaderDataType getType() const { return m_type; }
    bool isIntergerType() const;
    bool isNormalized() const { return m_isNormalized; }

  private:
    std::string m_name;
    ShaderDataType m_type;
    uint32_t m_size;
    uint32_t m_offset;
    bool m_isNormalized;
};

class BufferLayout
{
  public:
    BufferLayout() {}
    BufferLayout(const std::initializer_list<BufferElement> &list) : m_elements(list)
    {
        calculateElementsOffsets();
    }

    uint32_t getStride() const { return m_stride; }
    size_t getCount() const { return m_elements.size(); }

    std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_elements.end(); }

  private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride;

  private:
    void calculateElementsOffsets();
};

uint32_t getShaderTypeDataCount(const ShaderDataType &type);

//--buffer --//
class VertexBuffer
{
  public:
    virtual ~VertexBuffer() {}

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual BufferLayout &getBufferLayout()                      = 0;
    virtual void setBufferLayout(const BufferLayout &)           = 0;
    virtual void setData(uint32_t pos, void *data, uint32_t len) = 0;

    // 注意这个static函数是在基类声明的, 会根据当前Renderer::GetAPI()返回VertexBuffer的派生类对象
    // static VertexBuffer *Create(float *vertices, uint32_t size); // static buffer
    // static VertexBuffer *Create(uint32_t size);					 // dynamic buffer

    static std::shared_ptr<VertexBuffer> Create(float *vertices, uint32_t size);
    static std::shared_ptr<VertexBuffer> Create(uint32_t size);

  protected:
    uint32_t m_vertexBuffer;
};

class IndexBuffer
{
  public:
    virtual ~IndexBuffer() {}

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;
    static std::shared_ptr<IndexBuffer> Create(uint32_t *indices, uint32_t size);

  protected:
    uint32_t m_indexBuffer;
};
} // namespace Airwave
