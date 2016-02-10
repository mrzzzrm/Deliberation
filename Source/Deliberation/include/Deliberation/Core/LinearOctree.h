#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

template<typename NodeType>
class DELIBERATION_API LinearOctree final
{
public:
    static std::size_t levelBaseIndex(unsigned int level);

public:
    class DELIBERATION_API Iterator final
    {
    public:
        Iterator(LinearOctree & tree, uint32_t index);

        bool isValid() const;

        void toFirstChild();
        void toNextSibling();

        void toParent();

        NodeType & node() const;

    private:
        LinearOctree &  m_tree;
        uint32_t        m_index;
    };

public:
    LinearOctree(unsigned int depth);

    unsigned int depth() const;
    std::vector<NodeType> & nodes();
    const std::vector<NodeType> & nodes() const;

    Iterator root();

    NodeType & node(std::size_t index);
    const NodeType & node(std::size_t index) const;

    NodeType & leaf(uint32_t morton);
    const NodeType & leaf(uint32_t morton) const;

    NodeType & leaf(const glm::uvec3 & v);
    const NodeType & leaf(const glm::uvec3 & v) const;

private:
    unsigned int            m_depth;
    std::vector<NodeType>   m_nodes;
};

}

#include <Deliberation/Core/LinearOctree.inl>
