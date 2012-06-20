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

#define OUTOFRANGE_DISTANCE 100000

class Grid3D : public Grid
{
public:
    Grid3D(){};
    Grid3D(KDTree tree, const size_t dim_x, const size_t dim_y, const size_t dim_z);
    virtual ~Grid3D() {};

    virtual void approximateWLS(VertexList& resultList);
    virtual void draw();
	virtual double getInterpolatedFunctionValue(float x,float y,float z);
	virtual double getImplicitFunctionValueWorldCoordinates(float x,float y,float z);

	int getIndex(int idX,int idY,int idZ);
	shared_ptr<Vertex> getVertex(int idX,int idY,int idZ);
	double getVertexValue(int idX,int idY,int idZ);

	size_t m_dimX;
    size_t m_dimY;
    size_t m_dimZ;
	bool m_interpolate;

protected:
    void generateVertices();

private:
    unsigned int factorial(const int num);

    KDTree m_tree;
    KDTree m_fullTree;

    float m_diagLength;
    float m_MinX;
    float m_MinY;
    float m_MinZ;
    float m_MaxX;
    float m_MaxY;
    float m_MaxZ;

    VertexList m_GridVertices;
    VertexList m_generatedPoints;
};


#endif
