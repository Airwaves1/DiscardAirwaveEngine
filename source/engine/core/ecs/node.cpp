#include "node.hpp"

namespace Airwave
{

Node::Node() : m_uuid(UUID::Generate())
{
}

Node::~Node()
{
    for (auto &child : m_children)
    {
        child->m_parent.reset(); // 使用reset而非setParent(nullptr)，避免不必要的递归调用
    }
}

void Node::setParent(const std::shared_ptr<Node> &parent)
{
    if (m_parent.lock().get() == parent.get()) return;

    if (auto oldParent = m_parent.lock()) oldParent->removeChild(shared_from_this());

    m_parent = parent;

    if (parent) parent->addChild(shared_from_this());
}

std::shared_ptr<Node> Node::getParent() const { return m_parent.lock(); }

bool Node::hasParent() const { return !m_parent.expired(); }

void Node::addChild(const std::shared_ptr<Node> &child)
{
    if (hasChild(child) || child->getParent() == shared_from_this()) return;

    if (auto oldParent = child->getParent()) oldParent->removeChild(child);

    m_children.push_back(child);
    child->m_parent = shared_from_this(); // 避免递归调用
}

void Node::removeChild(const std::shared_ptr<Node> &child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it == m_children.end()) return;

    m_children.erase(it);
    child->m_parent.reset(); // 直接reset而非setParent(nullptr)，避免递归
}

bool Node::hasChild(const std::shared_ptr<Node> &child) const
{
    return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
}

} // namespace Airwave
