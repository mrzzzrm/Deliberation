#include <limits>

#include <sstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/Morton.h>
#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{
std::size_t LinearOctreeLevelBaseIndex(unsigned int level)
{
    switch (level)
    {
    case 0: return 0;
    case 1: return 1;
    case 2: return (1 << 3) | 1;
    case 3: return (1 << 6) | (1 << 3) | 1;
    case 4: return (1 << 9) | (1 << 6) | (1 << 3) | 1;
    case 5: return (1 << 12) | (1 << 9) | (1 << 6) | (1 << 3) | 1;
    case 6: return (1 << 15) | (1 << 12) | (1 << 9) | (1 << 6) | (1 << 3) | 1;
    case 7:
        return (1 << 18) | (1 << 15) | (1 << 12) | (1 << 9) | (1 << 6) |
               (1 << 3) | 1;
    case 8:
        return (1 << 21) | (1 << 18) | (1 << 15) | (1 << 12) | (1 << 9) |
               (1 << 6) | (1 << 3) | 1;
    case 9:
        return (1 << 24) | (1 << 21) | (1 << 18) | (1 << 15) | (1 << 12) |
               (1 << 9) | (1 << 6) | (1 << 3) | 1;
    default: Fail("");
    }
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::
    IteratorT(QualifiedTreeType & tree, uint32_t index)
    : m_tree(tree), m_index(index)
{
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
template<typename OtherIteratorT>
LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::
    IteratorT(const OtherIteratorT & iter)
    : m_tree(iter.m_tree), m_index(iter.m_index)
{
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
glm::uvec3
LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::cell()
    const
{
    return Morton3D(cellIndex()).toVec();
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
uint32_t LinearOctree<NodeType>::
    IteratorT<QualifiedNodeType, QualifiedTreeType>::cellIndex() const
{
    Assert(isCell());
    return m_index - LinearOctreeLevelBaseIndex(m_tree.depth() - 1);
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
uint32_t
LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::index()
    const
{
    return m_index;
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
uint32_t LinearOctree<NodeType>::
    IteratorT<QualifiedNodeType, QualifiedTreeType>::subnodeBit() const
{
    return 1 << ((m_index - 1) & 7);
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
QualifiedNodeType &
LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::node()
    const
{
    Assert(isValid());
    return m_tree.m_nodes[m_index];
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
bool LinearOctree<
    NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::isCell() const
{
    return m_index >= LinearOctreeLevelBaseIndex(m_tree.depth() - 1);
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
bool LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::
    hasParent() const
{
    return m_index != 0;
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
bool LinearOctree<
    NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::isValid() const
{
    return m_index != std::numeric_limits<decltype(m_index)>::max();
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
typename LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>
LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::
    firstChild() const
{
    auto iter = *this;
    iter.toFirstChild();
    return iter;
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
typename LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>
LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::
    nextSibling() const
{
    auto iter = *this;
    iter.toNextSibling();
    return iter;
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
typename LinearOctree<NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>
LinearOctree<
    NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::parent() const
{
    auto iter = *this;
    iter.toParent();
    return iter;
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
void LinearOctree<
    NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::toFirstChild()
{
    m_index = (m_index << 3) + 1;
    if (m_index >= m_tree.nodes().size())
    {
        m_index = std::numeric_limits<decltype(m_index)>::max();
    }
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
void LinearOctree<
    NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::toNextSibling()
{
    m_index++;
    if ((m_index & 7) == 1)
    {
        m_index = std::numeric_limits<decltype(m_index)>::max();
    }
}

template<typename NodeType>
template<typename QualifiedNodeType, typename QualifiedTreeType>
void LinearOctree<
    NodeType>::IteratorT<QualifiedNodeType, QualifiedTreeType>::toParent()
{
    if (m_index == 0)
    {
        m_index = std::numeric_limits<decltype(m_index)>::max();
    }

    m_index -= 1;
    m_index >>= 3;
}

template<typename NodeType>
LinearOctree<NodeType>::LinearOctree(unsigned int depth) : m_depth(depth)
{
    if (m_depth > 0)
    {
        m_nodes.resize(LinearOctreeLevelBaseIndex(m_depth));
    }
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
typename LinearOctree<NodeType>::CIterator LinearOctree<NodeType>::root() const
{
    return CIterator(*this, 0);
}

template<typename NodeType>
typename LinearOctree<NodeType>::Iterator
LinearOctree<NodeType>::cell(const glm::uvec3 & v)
{
    return cell(Morton3D(v).toIndex());
}

template<typename NodeType>
typename LinearOctree<NodeType>::CIterator
LinearOctree<NodeType>::cell(const glm::uvec3 & v) const
{
    return cell(Morton3D(v).toIndex());
}

template<typename NodeType>
typename LinearOctree<NodeType>::Iterator
LinearOctree<NodeType>::cell(std::size_t cellIndex)
{
    return node(LinearOctreeLevelBaseIndex(m_depth - 1) + cellIndex);
}

template<typename NodeType>
typename LinearOctree<NodeType>::CIterator
LinearOctree<NodeType>::cell(std::size_t cellIndex) const
{
    return node(LinearOctreeLevelBaseIndex(m_depth - 1) + cellIndex);
}

template<typename NodeType>
typename LinearOctree<NodeType>::Iterator
LinearOctree<NodeType>::node(std::size_t index)
{
    Assert(index < m_nodes.size());
    return Iterator(*this, index);
}
template<typename NodeType>
typename LinearOctree<NodeType>::CIterator
LinearOctree<NodeType>::node(std::size_t index) const
{
    Assert(index < m_nodes.size());
    return CIterator(*this, index);
}

template<typename NodeType>
std::string LinearOctree<NodeType>::toString() const
{
    std::stringstream stream;
    std::string       indent;
    auto              currentLevel = 0;

    for (auto n = 0; n < m_nodes.size(); n++)
    {
        if (n >= LinearOctreeLevelBaseIndex(currentLevel + 1))
        {
            currentLevel++;
            indent += "  ";
        }

        stream << indent << n << ": " << m_nodes[n] << std::endl;
    }

    return stream.str();
}
}
