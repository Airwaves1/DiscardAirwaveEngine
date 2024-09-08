#pragma once

#include "utils/common.hpp"
#include <memory>

namespace Airwave
{

class AwWindow;
class GraphicContext
{

  public:
    GraphicContext(const GraphicContext &)            = delete;
    GraphicContext &operator=(const GraphicContext &) = delete;

    virtual ~GraphicContext() = default;

    static std::unique_ptr<GraphicContext> Create(AwWindow *window);

  protected:
    GraphicContext() = default;
};

} // namespace Airwave