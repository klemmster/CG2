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

void HalfEdgeMesh::project(TriHalfEdgeMesh::VertexHandle& vertex){

    TriHalfEdgeMesh::Point point = m_Mesh.point(vertex);
    float delta = 0.1f;
    float funValue = m_Grid.getImplicitFunctionValueWorldCoordinates(point[0], point[1], point[2], 0);
    if(isnan(funValue)!= 0 || isinf(funValue)!= 0){
        std::cout << "Got unreal Value\n";
        return;
    }
    float xDelta = m_Grid.getImplicitFunctionValueWorldCoordinates(point[0]+delta, point[1], point[2], 0);
    if(isnan(xDelta)!= 0 || isinf(xDelta)!= 0){
        std::cout << "Got unreal Value XDelta\n";
        std::cout << "For Point: " << point << "\n";
        std::cout << "For ModPo: " << point[0]+delta << point[1] << point[2] << "\n";
        return;
    }
    if(xDelta == FLT_MAX){
        //std::cout << "floatMAX __ X\n";
        xDelta = delta;
    }else{
        //std::cout << "xDelta: " << xDelta << "\n";
        xDelta -= funValue;
    }
    float yDelta = m_Grid.getImplicitFunctionValueWorldCoordinates(point[0], point[1]+delta, point[2], 0);
    if(yDelta == FLT_MAX){
        yDelta = delta;
        //std::cout << "floatMAX __ Y\n";
    }else{
        //std::cout << "yDelta: " << yDelta << "\n";
        yDelta -= funValue;
    }

    float zDelta = m_Grid.getImplicitFunctionValueWorldCoordinates(point[0], point[1], point[2]+delta, 0);
    if(zDelta == FLT_MAX){
        zDelta = delta;
        //std::cout << "floatMAX __ Z\n";
    }else{
        //std::cout << "zDelta: " << zDelta << "\n";
        zDelta -= funValue;
    }


    float length = sqrt((xDelta*xDelta)+(yDelta*yDelta)+(zDelta*zDelta));

    if(isinf(length) !=0){
        std::cout << "xDelta: " << xDelta << "\n";
        std::cout << "yDelta: " << yDelta << "\n";
        std::cout << "zDelta: " << zDelta << "\n";
    }

    if(isnan(xDelta)!= 0 || isinf(xDelta)!= 0 || isnan(yDelta)!= 0 || isinf(yDelta)!= 0 ||
            isnan(zDelta)!= 0 || isinf(zDelta)!= 0 || isinf(length)!=0){
        std::cout << "Bogus Values\n";
        return;
    }

    if(length != 0){
        point[0] -= xDelta * funValue / length;
        point[1] -= yDelta * funValue / length;
        point[2] -= zDelta * funValue / length;
    }

    m_Mesh.set_point(vertex, point);

}

void HalfEdgeMesh::projectAll(){
    TriHalfEdgeMesh::VertexIter vrtxIT;
    for(vrtxIT = m_Mesh.vertices_begin(); vrtxIT!=m_Mesh.vertices_end(); ++vrtxIT){
        TriHalfEdgeMesh::VertexHandle vertex = vrtxIT.handle();
        project(vertex);
    }
}

void HalfEdgeMesh::loadFromFile(const std::string fileName){
    if (!OpenMesh::IO::read_mesh(m_Mesh, fileName))
    {
      std::cerr << "read error\n";
      exit(1);
    }
    m_Mesh.request_vertex_normals();
    m_Mesh.request_face_normals();
    m_Mesh.update_normals();
}

void HalfEdgeMesh::draw(bool wireframe){

    TriHalfEdgeMesh::FaceIter faceIT;
    TriHalfEdgeMesh::FaceVertexIter vrtxIT;

    TriHalfEdgeMesh::Normal normal;

    if(wireframe)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glColor3f(1.0f, 0.0f, 0.0f);
    m_Mesh.update_normals();
    glBegin(GL_TRIANGLES);
        for(faceIT=m_Mesh.faces_begin(); faceIT!= m_Mesh.faces_end(); ++faceIT){
            vrtxIT = m_Mesh.fv_iter(faceIT.handle());
            for(; vrtxIT; ++vrtxIT){
                glNormal3f(m_Mesh.normal(vrtxIT.handle())[0],m_Mesh.normal(vrtxIT.handle())[1],
                        m_Mesh.normal(vrtxIT.handle())[2]);
                glVertex3f(m_Mesh.point(vrtxIT.handle())[0],m_Mesh.point(vrtxIT.handle())[1],
                        m_Mesh.point(vrtxIT.handle())[2]);
            }
        }
    glEnd();
    if(wireframe)
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

