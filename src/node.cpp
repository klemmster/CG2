#include "node.hpp"

Node::Node(const VertexPtr position, const NodePtr left, const NodePtr right):
    m_position(position),
    m_left(left),
    m_right(right)
{};

Node::~Node(){};
