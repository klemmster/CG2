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

    // minimal greater bounding box
    m_MinX = (*minm_vertices.at(0))[0] - 0.01;
    m_MinY = (*minm_vertices.at(1))[1] - 0.01;
    m_MinZ = (*minm_vertices.at(2))[2] - 0.01;

    m_MaxX = (*maxm_vertices.at(0))[0] + 0.01;
    m_MaxY = (*maxm_vertices.at(1))[1] + 0.01;
    m_MaxZ = (*maxm_vertices.at(2))[2] + 0.01;

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
    //Single Approximation for every Grid point
    for(VertexPtr pointDesired: resultList)
    {
        //Get Points used for this approximation
        //TODO: Should be a good automatic? radius -- maybe gridsize related
        VertexList list = m_tree.findInRadius(pointDesired, m_radius);
        MatrixXf bDimsMatSum(6,6);
        VectorXf bDimsVecSum(6);
        bDimsMatSum <<  0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0;
        bDimsVecSum << 0, 0, 0, 0, 0, 0;

        for(VertexPtr point : list)
        {
            VectorXf b(6);
            float x = (*point)[0];
            float y = (*point)[1];
            float z = (*point)[2];
            b << 1, x, y, x*x, x*y, y*y;
            MatrixXf bDims(6,6);
            bDims = b*b.transpose();
            vec3f distVec = (*pointDesired) - (*point);
            float dist = norm(distVec);
            float wendFac = getWendland(dist);
            bDimsMatSum += (wendFac * bDims);
            bDimsVecSum += (wendFac * b*z);
        }

        // constant polynom basis
        /**
        VectorXf b(1);
        float x = (*pointDesired)[0];
        float y = (*pointDesired)[1];
        float z = (*pointDesired)[2];
        b << 1;
        **/

        // linear polynom basis
        /**
        VectorXf b(4);
        float x = (*pointDesired)[0];
        float y = (*pointDesired)[1];
        float z = (*pointDesired)[2];
        b << 1, x, y, z;
        **/

        // qadratic polynom basis
        /**
        VectorXf b(10);
        float x = (*pointDesired)[0];
        float y = (*pointDesired)[1];
        float z = (*pointDesired)[2];
        b << 1, x, y, z, x*x, x*y, x*z, y*y, y*z, z*z;
        **/

        VectorXf c(6);
        c = bDimsMatSum.inverse() * bDimsVecSum;
        (*pointDesired)[2] = b.dot(c);
        m_coefficients.push_back(c);
    }
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
