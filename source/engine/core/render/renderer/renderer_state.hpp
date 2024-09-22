#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Airwave
{
enum class RenderState : uint8_t
{
    None        = 0,      // 0000 0000
    DepthTest   = 1 << 0, // 0000 0001
    DepthWrite  = 1 << 1, // 0000 0010
    Blend       = 1 << 2, // 0000 0100
    CullFace    = 1 << 3, // 0000 1000
    ScissorTest = 1 << 4, // 0001 0000
    StencilTest = 1 << 5, // 0010 0000
    Multisample = 1 << 6, // 0100 0000
    LineSmooth  = 1 << 7, // 1000 0000
};

enum class RenderPolygonMode : uint8_t
{
    Fill  = 0,
    Line  = 1,
    Point = 2,
};

enum class RenderClearFlag : uint8_t
{
    Color   = 1 << 0,
    Depth   = 1 << 1,
    Stencil = 1 << 2,
};

// 重载按位运算符
inline RenderState operator|(RenderState lhs, RenderState rhs)
{
    return static_cast<RenderState>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline RenderClearFlag operator|(RenderClearFlag lhs, RenderClearFlag rhs)
{
    return static_cast<RenderClearFlag>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

inline RenderState operator&(RenderState lhs, RenderState rhs)
{
    return static_cast<RenderState>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline RenderState operator^(RenderState lhs, RenderState rhs)
{
    return static_cast<RenderState>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
}

inline RenderClearFlag operator&(RenderClearFlag lhs, RenderClearFlag rhs)
{
    return static_cast<RenderClearFlag>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

class RenderStateManager
{
  public:
    static uint8_t GetCurrentRenderState(RenderState state) { return static_cast<uint8_t>(state); }
    static uint8_t GetCurrentClearFlag(RenderClearFlag flag) { return static_cast<uint8_t>(flag); }

    static std::vector<std::string> GetRenderStateStrings(RenderState state);
    static std::vector<std::string> GetClearFlagStrings(RenderClearFlag flag);

    static std::string GetRenderStateString(RenderState state);
    static std::string GetClearFlagString(RenderClearFlag flag);

    static std::string GetRenderPolygonModeString(RenderPolygonMode mode);
    
};

} // namespace Airwave
