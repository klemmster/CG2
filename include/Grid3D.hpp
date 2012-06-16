//
//  Grid3D.hpp
//  PointMesh
//
//  Created by Christopher Sierigk on 14.06.12.
//  Copyright (c) 2012 Smart Mobile Factory. All rights reserved.
//

#ifndef PointMesh_Grid3D_hpp
#define PointMesh_Grid3D_hpp

#include "Grid.hpp"
#include "tree.hpp"
#include "vertex.hpp"

class Grid3D : public Grid
{
public:
    Grid3D(){};
    Grid3D(KDTree tree, const size_t dim_x, const size_t dim_y, const size_t dim_z);
    virtual ~Grid3D() {};

    virtual void approximateWLS(VertexList& resultList);
    virtual void draw();

private:
    unsigned int factorial(const int num);

    KDTree m_tree;
    size_t m_dimX;
    size_t m_dimY;
    size_t m_dimZ;

    float m_MinX;
    float m_MinY;
    float m_MinZ;
    float m_MaxX;
    float m_MaxY;
    float m_MaxZ;

    VertexList m_vertices;
};


#endif
