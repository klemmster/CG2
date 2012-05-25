#include "node.hpp"

Node::Node(const NodePtr left, const NodePtr right, const Domain domain):
    m_left(left),
    m_right(right),
    m_domain(domain)
{ };

Node::Node(const VertexList bucket, const Domain domain):
    m_Bucket(bucket),
    m_left(nullptr),
    m_right(nullptr),
    m_domain(domain){

    };

Node::~Node(){}

const Domain& Node::getDomain(){
    return m_domain;
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
