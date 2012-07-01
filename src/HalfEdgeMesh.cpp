//
//  HalfEdgeMesh.cpp
//  PointMesh
//
//  Created by Christopher Sierigk on 01.07.12.
//  Copyright (c) 2012 Smart Mobile Factory. All rights reserved.
//

#include "HalfEdgeMesh.hpp"

HalfEdgeMesh::HalfEdgeMesh()
{
    
}

HalfEdgeMesh::~HalfEdgeMesh()
{
    
}

void HalfEdgeMesh::generateHalfEdge(const VertexList& triangles)
{
    TriHalfEdgeMesh::VertexHandle vHandle[3];
    vector<TriHalfEdgeMesh::VertexHandle> face_vHandles;
    
    cout << "No of Faces " << triangles.size() / 3.0f << "\n";
    
    for (size_t i = 0; i < triangles.size(); i += 3)
    {
        vHandle[0] = m_Mesh.add_vertex(TriHalfEdgeMesh::Point(triangles.at(i)->_v));
        vHandle[1] = m_Mesh.add_vertex(TriHalfEdgeMesh::Point(triangles.at(i+1)->_v));
        vHandle[2] = m_Mesh.add_vertex(TriHalfEdgeMesh::Point(triangles.at(i+2)->_v));
        face_vHandles.clear();
        face_vHandles.push_back(vHandle[0]);
        face_vHandles.push_back(vHandle[1]);
        face_vHandles.push_back(vHandle[2]);
        m_Mesh.add_face(face_vHandles);
    }
    
    try {
        if (!OpenMesh::IO::write_mesh(m_Mesh, "mesh.off"))
        {
            std::cerr << "Cannot write mesh to file" << std::endl;
        }
    } catch (std::exception& x) {
        std::cerr << x.what() << std::endl;
    }
}