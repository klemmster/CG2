#include "leaf.hpp"

Leaf::Leaf(const VertexPtr position, const LeafPtr left, const LeafPtr right):
    m_position(position),
    m_left(left),
    m_right(right)
{};

Leaf::~Leaf(){};
