//
//  HalfEdgeMesh.cpp
//  PointMesh
//
//  Created by Christopher Sierigk on 01.07.12.
//  Copyright (c) 2012 Smart Mobile Factory. All rights reserved.
//
#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif


#include "HalfEdgeMesh.hpp"

HalfEdgeMesh::HalfEdgeMesh()
{

}

HalfEdgeMesh::~HalfEdgeMesh()
{

}

void HalfEdgeMesh::generateHalfEdge(const VertexList& triangles)
{
    //Sort vertices and make vertixes unique
    VertexList uniqueList;
    std::vector<size_t> indices;
    indices.reserve(triangles.size());

    float maxDist = 0.01f;
    for(VertexPtr candidate: triangles){
        bool found = false;
        for(int i = 0; i<uniqueList.size(); ++i){
            VertexPtr uniquePtr = uniqueList.at(i);
            vec3f distVec = (*candidate) - (*uniquePtr);
            float dist = norm(distVec);
            if(dist < maxDist){
                indices.push_back(i);
                found = true;
                break;
            }
        }
        if(!found){
            indices.push_back(uniqueList.size());
            uniqueList.push_back(candidate);
        }
    }


    vector<TriHalfEdgeMesh::VertexHandle> vertexHandles;
    for(VertexPtr vrtx: uniqueList){
        vertexHandles.push_back(m_Mesh.add_vertex(TriHalfEdgeMesh::Point(vrtx->_v)));
    }

    vector<TriHalfEdgeMesh::VertexHandle> face_vHandles;

    cout << "No of Faces " << triangles.size() / 3.0f << "\n";

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        face_vHandles.clear();
        face_vHandles.push_back(vertexHandles.at(indices.at(i)));
        face_vHandles.push_back(vertexHandles.at(indices.at(i+1)));
        face_vHandles.push_back(vertexHandles.at(indices.at(i+2)));
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

void HalfEdgeMesh::loadFromFile(const std::string fileName){
    if (!OpenMesh::IO::read_mesh(m_Mesh, fileName))
    {
      std::cerr << "read error\n";
      exit(1);
    }
}

void HalfEdgeMesh::draw(){

    TriHalfEdgeMesh::FaceIter faceIT;
    TriHalfEdgeMesh::FaceVertexIter vrtxIT;

	glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        for(faceIT=m_Mesh.faces_begin(); faceIT!= m_Mesh.faces_end(); ++faceIT){
            vrtxIT = m_Mesh.fv_iter(faceIT.handle());
            for(; vrtxIT; ++vrtxIT){
                glVertex3f(m_Mesh.point(vrtxIT.handle())[0],m_Mesh.point(vrtxIT.handle())[1],
                        m_Mesh.point(vrtxIT.handle())[2]);
            }
        }
    glEnd();
}
