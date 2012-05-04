#include "node.hpp"

Node::Node(const VertexPtr position, const NodePtr left, const NodePtr right,
        const Boundaries boundaries):
    m_position(position),
    m_left(left),
    m_right(right),
    m_boundaries(boundaries)
{};

Node::~Node(){}

const Boundaries& Node::getBoundaries(){
    return m_boundaries;
}
