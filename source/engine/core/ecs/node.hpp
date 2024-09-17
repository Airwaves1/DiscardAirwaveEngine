#pragma once

/**
 * 用于实现一个树形结构，代表场景中的层级关系。每个Node可以有父节点和多个子节点，
 * 适用于组织和管理复杂的对象层次结构。
 * 支持设置和获取父节点，添加、移除子节点，检查是否有父节点或子节点
 * Node有一个名称属性，可以设置和获取
 */

#include "uuid.hpp"
#include <vector>
#include <memory>

namespace Airwave
{
class Node : public std::enable_shared_from_this<Node>
{

  public:
    Node();
    virtual ~Node(); 

    UUID getUUID() const { return m_uuid; }
    std::string getName() const { return m_name; }
    void setName(const std::string &name) { m_name = name; }

    std::shared_ptr<Node> getParent() const;
    void setParent(const std::shared_ptr<Node> &parent);
    bool hasParent() const;

    const std::vector<std::shared_ptr<Node>> &getChildren() const;
    void addChild(const std::shared_ptr<Node> &child);
    void removeChild(const std::shared_ptr<Node> &child);
    bool hasChild(const std::shared_ptr<Node> &child) const;
    bool hasChildren() const { return !m_children.empty(); }

  private:
    UUID m_uuid;
    std::string m_name{"Node"};
    std::weak_ptr<Node> m_parent;
    std::vector<std::shared_ptr<Node>> m_children;
};
} // namespace Airwave