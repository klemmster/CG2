#include "vertex.hpp"

#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif



Vertex::Vertex(const vec3f pos):
    vec3f(pos)
{
    m_Color = vec3f(.6, .6, .6);
}

Vertex::Vertex(const float x, const float y, const float z):
    vec3f(x, y, z)
{
    m_Color = vec3f(.6, .6, .6);
}

Vertex::Vertex(const float x, const float y, const float z, const vec3f color):
    vec3f(x, y, z),
    m_Color(color)
{

}

Vertex::~Vertex()
{
     //dtor
}

void Vertex::draw() const{
    //glColor3fv(m_Color._v);
    glVertex3fv(_v);
}

void Vertex::highlight(const vec3f color){
    m_Color = color;
}

void Vertex::resetColor(){
    m_Color = vec3f(.6, .6, .6);
}
