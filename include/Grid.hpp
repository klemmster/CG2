#ifndef GRID_HPP_FY7TIIFJ
#define GRID_HPP_FY7TIIFJ

#include "tree.hpp"

class Grid
{
public:
    Grid () {};
    Grid (KDTree tree, const size_t dim_x, const size_t dim_y);
    virtual ~Grid () {};

    void approximateLS();
    void draw();

private:

    float m_MinX;
    float m_MinY;
    float m_MinZ;
    float m_MaxX;
    float m_MaxY;
    float m_MaxZ;

    VertexList m_vertices;
    KDTree m_tree;
    size_t m_dimX;
    size_t m_dimY;

};

#endif /* end of include guard: GRID_HPP_FY7TIIFJ */

