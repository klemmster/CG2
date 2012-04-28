#ifndef LEAF_HPP_QAN5IUDG
#define LEAF_HPP_QAN5IUDG

#include "vertex.hpp"

class Leaf
{
public:
    Leaf (const VertexPtr position, const VertexPtr left, const VertexPtr right);
    virtual ~Leaf ();

private:

    Leaf() {};
    const VertexPtr m_position;
    const VertexPtr m_left;
    const VertexPtr m_right;
};

typedef std::shared_ptr< Leaf > LeafPtr;

#endif /* end of include guard: LEAF_HPP_QAN5IUDG */

