#include "leaf.hpp"

Leaf::Leaf(const VertexPtr position, const VertexPtr left, const VertexPtr right):
    m_position(position),
    m_left(left),
    m_right(right)
{};

Leaf::~Leaf(){};
