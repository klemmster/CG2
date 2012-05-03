#ifndef NODE_HPP_QAN5IUDG
#define NODE_HPP_QAN5IUDG

#include "vertex.hpp"

using namespace std;

class Node;
typedef shared_ptr< Node > NodePtr;

class Node
{
public:
    Node (const VertexPtr position, const NodePtr left, const NodePtr right);
    virtual ~Node ();

private:

    Node() {};
    const VertexPtr m_position;
    const NodePtr m_left;
    const NodePtr m_right;
};

#endif /* end of include guard: NODE_HPP_QAN5IUDG */

