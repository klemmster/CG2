#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

#include "Grid.hpp"
#include "vertex.hpp"


Grid::Grid (const KDTree tree, const size_t dim_x, const size_t dim_y){

    const VertexList& minm_vertices = tree.getMinVertices();
    const VertexList& maxm_vertices = tree.getMaxVertices();

    m_MinX = (*minm_vertices.at(0))[0];
    m_MinY = (*minm_vertices.at(1))[1];
    m_MinZ = (*minm_vertices.at(2))[2];

    m_MaxX = (*maxm_vertices.at(0))[0];
    m_MaxY = (*maxm_vertices.at(1))[1];
    m_MaxZ = (*maxm_vertices.at(2))[2];

    float xStep = abs(m_MaxX - m_MinX) / dim_x;
    float yStep = abs(m_MaxY - m_MinY) / dim_y;

    float xPos = m_MinX;
    float yPos = m_MinY;

    vec3f color(0.2, 0.8, 0.5);

    for(size_t y = 0; y < dim_y; ++y){
        for(size_t x = 0; x < dim_x; ++x){
            m_vertices.push_back(VertexPtr(new Vertex(xPos, yPos, m_MaxZ, color)));
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
    for(VertexPtr vrtx: m_vertices){
        vrtx->draw();
    }
    glEnd();
}

