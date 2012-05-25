#ifndef NODE_HPP_QAN5IUDG
#define NODE_HPP_QAN5IUDG

#include <array>

#include "vertex.hpp"
#include "domain.hpp"


using namespace std;

class Node;
typedef shared_ptr< Node > NodePtr;


class Node
{
public:
    /*Create a Node */
    Node(const NodePtr left, const NodePtr right, const Domain domain);
    /*Implicitly create a leave */
    Node(const VertexList bucket, const Domain domain);
    virtual ~Node ();

    const Domain& getDomain();
    const VertexList getBucket() const;
    const NodePtr getLeft() { return m_left;};
    const NodePtr getRight() { return m_right;};
    bool isLeaf() const;

private:

    Node() {};
    const VertexList m_Bucket;
    const NodePtr m_left;
    const NodePtr m_right;
    Domain m_domain;
};

#endif /* end of include guard: NODE_HPP_QAN5IUDG */

