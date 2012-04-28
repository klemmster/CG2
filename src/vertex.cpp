#include "vertex.hpp"

Vertex::Vertex(const vec3f pos):
    vec3f(pos)
{}

Vertex::Vertex(const float x, const float y, const float z):
     vec3f(x, y, z)
{}

Vertex::~Vertex()
{
     //dtor
}

