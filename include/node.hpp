#ifndef NODE_HPP_QAN5IUDG
#define NODE_HPP_QAN5IUDG

#include <array>

#include "vertex.hpp"


using namespace std;

class Node;
typedef shared_ptr< Node > NodePtr;
// xlow, xhigh, ylow, yhigh, zlow, zHigh
typedef std::array<float, 6> Boundaries;

class Node
{
public:
    Node (const VertexPtr position, const NodePtr left, const NodePtr right,
            const Boundaries boundaries);
    const Boundaries& getBoundaries();
    const VertexPtr& getPosition() {return m_position;};
    const NodePtr getLeft() { return m_left;};
    const NodePtr getRight() { return m_right;};
    virtual ~Node ();

private:

    Node() {};
    const VertexPtr m_position;
    const NodePtr m_left;
    const NodePtr m_right;
    Boundaries m_boundaries;
};

#endif /* end of include guard: NODE_HPP_QAN5IUDG */

