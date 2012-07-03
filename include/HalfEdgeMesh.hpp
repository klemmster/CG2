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

typedef OpenMesh::TriMesh_ArrayKernelT<> TriHalfEdgeMesh;

class HalfEdgeMesh
{
public:
    HalfEdgeMesh();
    virtual ~HalfEdgeMesh();

    void generateHalfEdge(const VertexList& triangles);
    void loadFromFile(const std::string fileName);
    TriHalfEdgeMesh getMesh() { return m_Mesh; }
    void draw();

private:
    TriHalfEdgeMesh m_Mesh;
};

#endif
