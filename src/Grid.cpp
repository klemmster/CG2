#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

#include "Grid.hpp"


Grid::Grid (const VertexList& minVertices, const VertexList& maxVertices){

    m_MinX = (*minVertices.at(0))[0];
    m_MinY = (*minVertices.at(1))[1];
    m_MinZ = (*minVertices.at(2))[2];

    m_MaxX = (*maxVertices.at(0))[0];
    m_MaxY = (*maxVertices.at(1))[1];
    m_MaxZ = (*maxVertices.at(2))[2];
}

void Grid::draw(){

    glBegin(GL_LINES);

    glVertex3f(m_MinX, m_MinY, 0);
    glVertex3f(m_MaxX, m_MinY, 0);

    glVertex3f(m_MinX, m_MinY, 0);
    glVertex3f(m_MinX, m_MaxY, 0);

    glEnd();

}
