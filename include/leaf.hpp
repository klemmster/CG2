#ifndef LEAF_HPP_QAN5IUDG
#define LEAF_HPP_QAN5IUDG

#include "vertex.hpp"

using namespace std;

class Leaf;
typedef shared_ptr< Leaf > LeafPtr;

class Leaf
{
public:
    Leaf (const VertexPtr position, const LeafPtr left, const LeafPtr right);
    virtual ~Leaf ();

private:

    Leaf() {};
    const VertexPtr m_position;
    const LeafPtr m_left;
    const LeafPtr m_right;
};

#endif /* end of include guard: LEAF_HPP_QAN5IUDG */

