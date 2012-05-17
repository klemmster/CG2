#ifndef GRID_HPP_FY7TIIFJ
#define GRID_HPP_FY7TIIFJ

#include "vertex.hpp"

class Grid
{
public:
    Grid () {};
    Grid (const VertexList& minVertices, const VertexList& maxVertices);
    virtual ~Grid () {};

    void draw();

private:

    float m_MinX;
    float m_MinY;
    float m_MinZ;
    float m_MaxX;
    float m_MaxY;
    float m_MaxZ;

};

#endif /* end of include guard: GRID_HPP_FY7TIIFJ */

