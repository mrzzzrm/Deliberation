#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <string>
#include <vector>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

inline std::size_t LinearOctreeLevelBaseIndex(unsigned int level);

template<typename NodeType>
class DELIBERATION_API LinearOctree final
{
public:
    template<typename QualifiedNodeType, typename QualifiedTreeType>
    class DELIBERATION_API IteratorT final
    {
    public:
        typedef QualifiedNodeType Node;

    public:
        IteratorT(QualifiedTreeType & tree, uint32_t index);

        template<typename OtherIteratorT>
        IteratorT(const OtherIteratorT & other);

        glm::uvec3 cell() const;
        uint32_t index() const;
        uint32_t cellIndex() const;
        uint32_t subnodeBit() const;
        QualifiedNodeType & node() const;

        bool isCell() const;
        bool hasParent() const;
        bool isValid() const;

        IteratorT<QualifiedNodeType, QualifiedTreeType> firstChild() const;
        IteratorT nextSibling() const;
        IteratorT parent() const;

        void toFirstChild();
        void toNextSibling();
        void toParent();

    private:
        QualifiedTreeType & m_tree;
        uint32_t            m_index;
    };

    typedef IteratorT<NodeType, LinearOctree> Iterator;
    typedef IteratorT<const NodeType, const LinearOctree> CIterator;

public:
    LinearOctree(unsigned int depth = 0);

    unsigned int depth() const;

    std::vector<NodeType> & nodes();
    const std::vector<NodeType> & nodes() const;

    Iterator root();
    CIterator root() const;

    Iterator cell(const glm::uvec3 & v);
    CIterator cell(const glm::uvec3 & v) const;

    Iterator cell(std::size_t cellIndex);
    CIterator cell(std::size_t cellIndex) const;

    Iterator node(std::size_t index);
    CIterator node(std::size_t index) const;

    std::string toString() const;

private:
    unsigned int            m_depth;
    std::vector<NodeType>   m_nodes;
};

}

#include <Deliberation/Core/LinearOctree.inl>
