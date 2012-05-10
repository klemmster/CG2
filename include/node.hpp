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
    /*Create a Node */
    Node(const NodePtr left, const NodePtr right, const Boundaries boundaries);
    /*Implicitly create a leave */
    Node(const VertexList bucket, const Boundaries boundaries);
    virtual ~Node ();

    const Boundaries& getBoundaries();
    const VertexList getBucket() const;
    const NodePtr getLeft() { return m_left;};
    const NodePtr getRight() { return m_right;};
    bool isLeaf() const;

private:

    Node() {};
    const VertexList m_Bucket;
    const NodePtr m_left;
    const NodePtr m_right;
    Boundaries m_boundaries;
};

#endif /* end of include guard: NODE_HPP_QAN5IUDG */

