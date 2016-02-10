#include <limits>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Math/Morton.h>

namespace deliberation
{

template<typename NodeType>
std::size_t LinearOctree<NodeType>::levelBaseIndex(unsigned int level)
{
    switch(level)
    {
    case 0: return 0;
    case 1: return 1;
    case 2: return (1 << 3) | 1;
    case 3: return (1 << 6) | (1 << 3) | 1;
    case 4: return (1 << 9) | (1 << 6) | (1 << 3) | 1;
    case 5: return (1 << 12) | (1 << 9) | (1 << 6) | (1 << 3) | 1;
    case 6: return (1 << 15) | (1 << 12) | (1 << 9) | (1 << 6) | (1 << 3) | 1;
    case 7: return (1 << 18) | (1 << 15) | (1 << 12) | (1 << 9) | (1 << 6) | (1 << 3) | 1;
    case 8: return (1 << 21) | (1 << 18) | (1 << 15) | (1 << 12) | (1 << 9) | (1 << 6) | (1 << 3) | 1;
    case 9: return (1 << 24) | (1 << 21) | (1 << 18) | (1 << 15) | (1 << 12) | (1 << 9) | (1 << 6) | (1 << 3) | 1;
    default:
        Fail("");
    }
}

template<typename NodeType>
LinearOctree<NodeType>::Iterator::Iterator(LinearOctree & tree, uint32_t index):
    m_tree(tree),
    m_index(index)
{
}

template<typename NodeType>
bool LinearOctree<NodeType>::Iterator::isValid() const
{
    return m_index != std::numeric_limits<decltype(m_index)>::max();
}

template<typename NodeType>
void LinearOctree<NodeType>::Iterator::toFirstChild()
{
    m_index = (m_index << 3) + 1;
    if (m_index >= m_tree.nodes().size())
    {
        m_index = std::numeric_limits<decltype(m_index)>::max();
    }
}

template<typename NodeType>
void LinearOctree<NodeType>::Iterator::toNextSibling()
{
    m_index++;
    if ((m_index & 7) == 1)
    {
        m_index = std::numeric_limits<decltype(m_index)>::max();
    }
}

template<typename NodeType>
void LinearOctree<NodeType>::Iterator::toParent()
{
    if (m_index == 0)
    {
        m_index = std::numeric_limits<decltype(m_index)>::max();
    }

    m_index >>= 3;
}

template<typename NodeType>
NodeType & LinearOctree<NodeType>::Iterator::node() const
{
    return m_tree.node(m_index);
}

template<typename NodeType>
LinearOctree<NodeType>::LinearOctree(unsigned int depth):
    m_depth(depth)
{
    Assert(m_depth > 0, "");
    m_nodes.resize(levelBaseIndex(m_depth));
}

template<typename NodeType>
unsigned int LinearOctree<NodeType>::depth() const
{
    return m_depth;
}

template<typename NodeType>
std::vector<NodeType> & LinearOctree<NodeType>::nodes()
{
    return m_nodes;
}

template<typename NodeType>
const std::vector<NodeType> & LinearOctree<NodeType>::nodes() const
{
    return m_nodes;
}

template<typename NodeType>
typename LinearOctree<NodeType>::Iterator LinearOctree<NodeType>::root()
{
    return Iterator(*this, 0);
}

template<typename NodeType>
NodeType & LinearOctree<NodeType>::node(std::size_t index)
{
    Assert(index < m_nodes.size(), "");
    return m_nodes[index];
}

template<typename NodeType>
const NodeType & LinearOctree<NodeType>::node(std::size_t index) const
{
    Assert(index < m_nodes.size(), "");
    return m_nodes[index];
}

template<typename NodeType>
NodeType & LinearOctree<NodeType>::leaf(uint32_t morton)
{
    return node(levelBaseIndex(m_depth - 1) + morton);
}

template<typename NodeType>
const NodeType & LinearOctree<NodeType>::leaf(uint32_t morton) const
{
    return node(levelBaseIndex(m_depth - 1) + morton);
}

template<typename NodeType>
NodeType & LinearOctree<NodeType>::leaf(const glm::uvec3 & v)
{
    return node(levelBaseIndex(m_depth - 1) + Morton3D(v).toIndex());
}

template<typename NodeType>
const NodeType & LinearOctree<NodeType>::leaf(const glm::uvec3 & v) const
{
    return node(levelBaseIndex(m_depth - 1) + Morton3D(v).toIndex());
}

}

