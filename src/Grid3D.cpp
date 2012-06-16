//
//  Grid3D.cpp
//  PointMesh
//
//  Created by Christopher Sierigk on 14.06.12.
//  Copyright (c) 2012 Smart Mobile Factory. All rights reserved.
//

#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

#include <math.h>
#include "Grid3D.hpp"

Grid3D::Grid3D(KDTree tree, const size_t dim_x, const size_t dim_y, const size_t dim_z)
    : m_tree(tree), m_dimX(dim_x), m_dimY(dim_y), m_dimZ(dim_z)
{
    const VertexList& minm_vertices = m_tree.getMinVertices();
    const VertexList& maxm_vertices = m_tree.getMaxVertices();

    m_MinX = (*minm_vertices.at(0))[0];
    m_MinY = (*minm_vertices.at(1))[1];
    m_MinZ = (*minm_vertices.at(2))[2];

    m_MaxX = (*maxm_vertices.at(0))[0];
    m_MaxY = (*maxm_vertices.at(1))[1];
    m_MaxZ = (*maxm_vertices.at(2))[2];

    float stepX = abs(m_MaxX - m_MinX) / dim_x;
    float stepY = abs(m_MaxY - m_MinY) / dim_y;
    float stepZ = abs(m_MaxZ - m_MinZ) / dim_z;

    float xPos = m_MinX;
    float yPos = m_MinY;
    float zPos = m_MinZ;

    vec3f color(0.2, 0.8, 0.5);

    for (size_t z = 0; z <= m_dimZ; z++)
    {
        for (size_t y = 0; y <= m_dimY; y++)
        {
            for (size_t x = 0; x <= m_dimX; x++)
            {
                m_vertices.push_back(VertexPtr(new Vertex(xPos, yPos, zPos, color, 0)));
                xPos += stepX;
            }

            yPos += stepY;
            xPos = m_MinX;
        }

        zPos += stepZ;
        yPos = m_MinY;
    }
 }

void Grid3D::approximateWLS(VertexList& resultList)
{

}

void Grid3D::draw()
{
    glDisable(GL_LIGHTING);
    glColor3f(0, 0, 1);

    for (size_t i = 0; i < m_vertices.size(); i++)
    {
        VertexPtr point = m_vertices.at(i);

        glBegin(GL_POINTS);
            glVertex3fv(point->_v);
        glEnd();
    }

    glEnable(GL_LIGHTING);
}
