#include "vertex.hpp"

Vertex::Vertex(const vec3f pos):
    m_pos(pos)
{}

Vertex::Vertex(const float x, const float y, const float z):
     m_pos(vec3f(x, y, z))
{}

Vertex::~Vertex()
{
     //dtor
}

