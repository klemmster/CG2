#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

#include "Grid.hpp"
#include "vertex.hpp"


Grid::Grid (const VertexList& minVertices, const VertexList& maxVertices,
        const size_t dim_x, const size_t dim_y){

    m_MinX = (*minVertices.at(0))[0];
    m_MinY = (*minVertices.at(1))[1];
    m_MinZ = (*minVertices.at(2))[2];

    m_MaxX = (*maxVertices.at(0))[0];
    m_MaxY = (*maxVertices.at(1))[1];
    m_MaxZ = (*maxVertices.at(2))[2];

    float xStep = (abs(m_MaxX) - abs(m_MinX)) / dim_x;
    float yStep = (abs(m_MaxY) - abs(m_MinY)) / dim_y;

    float xPos = m_MinX;
    float yPos = m_MinY;

    vec3f color(0.2, 0.8, 0.5);

    for(size_t y = 0; y < dim_y; ++y){
        for(size_t x = 0; x < dim_x; ++x){
            vertices.push_back(VertexPtr(new Vertex(xPos, yPos, m_MaxZ, color)));
            xPos += xStep;
        }
            yPos += yStep;
            xPos = m_MinX;
    }


}

void Grid::draw(){

    glBegin(GL_LINE_STRIP);
        glVertex3f(m_MinX, m_MinY, m_MinZ);
        glVertex3f(m_MaxX, m_MinY, m_MinZ);
        glVertex3f(m_MaxX, m_MaxY, m_MinZ);
        glVertex3f(m_MinX, m_MaxY, m_MinZ);
        glVertex3f(m_MinX, m_MinY, m_MinZ);
    glEnd();

    glBegin(GL_POINTS);
    for(VertexPtr vrtx: vertices){
        vrtx->draw();
    }
    glEnd();
}

