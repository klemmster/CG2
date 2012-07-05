//
//  HalfEdgeMesh.h
//  PointMesh
//
//  Created by Christopher Sierigk on 01.07.12.
//  Copyright (c) 2012 Smart Mobile Factory. All rights reserved.
//

#ifndef PointMesh_HalfEdgeMesh_h
#define PointMesh_HalfEdgeMesh_h

#define OM_STATIC_BUILD

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "vertex.hpp"
#include "Grid3D.hpp"

typedef OpenMesh::TriMesh_ArrayKernelT<> TriHalfEdgeMesh;

class HalfEdgeMesh
{
public:
    HalfEdgeMesh();
    virtual ~HalfEdgeMesh();

    void generateHalfEdge(const VertexList& triangles);
    void loadFromFile(const std::string fileName);
    void setGrid(Grid3D grid) {m_Grid = grid;};
    void projectAll();
    float getQuality(const TriHalfEdgeMesh::FaceHandle& face);
    float getRingQuality(const TriHalfEdgeMesh::VertexHandle& vrtxHandle);
    float getEdgeQuality(const TriHalfEdgeMesh::EdgeHandle& edgeHandle);
    float getMeshQuality();
    void optimizeMesh(size_t numIter=20);
    TriHalfEdgeMesh getMesh() { return m_Mesh; }
    void draw(bool wireframe);

private:

    void project(TriHalfEdgeMesh::VertexHandle& vertex);
    void optimizeByFlip();
    void optimizeBySplit();
    void optimizeByCollapse();
    void optimizeByShift();
    TriHalfEdgeMesh m_Mesh;
    Grid3D m_Grid;
};

#endif
