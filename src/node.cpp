#include "node.hpp"

Node::Node(const NodePtr left, const NodePtr right, const Boundaries boundaries):
    m_left(left),
    m_right(right),
    m_boundaries(boundaries)
{ };

Node::Node(const VertexList bucket, const Boundaries boundaries):
    m_Bucket(bucket),
    m_left(nullptr),
    m_right(nullptr),
    m_boundaries(boundaries){

    };

Node::~Node(){}

const Boundaries& Node::getBoundaries(){
    return m_boundaries;
};

const VertexList Node::getBucket() const{
    return m_Bucket;
}

bool Node::isLeaf() const{
    if(m_left == nullptr && m_right == nullptr){
        return true;
    }
    return false;
}
