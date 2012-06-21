#include "vertex.hpp"

#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

Vertex::Vertex(const vec3f pos):
    vec3f(pos),
    m_Color(vec3f(.6f, .6f, .6f)),
    m_NormalPtr(nullptr),
    m_W(0.0)
{
}

Vertex::Vertex(const vec3f pos, NormalPtr normal):
    vec3f(pos),
    m_Color(vec3f(.6f, .6f, .6f)),
    m_NormalPtr(normal),
    m_W(0.0)
{
}

Vertex::Vertex(const float x, const float y, const float z):
    vec3f(x, y, z),
    m_Color(vec3f(.6f, .6f, .6f)),
    m_NormalPtr(nullptr),
    m_W(0.0)
{
}

Vertex::Vertex(const float x, const float y, const float z, NormalPtr normal):
    vec3f(x, y, z),
    m_Color(vec3f(.6f, .6f, .6f)),
    m_NormalPtr(normal),
    m_W(0.0)
{
}

Vertex::Vertex(const float x, const float y, const float z, const vec3f color):
    vec3f(x, y, z),
    m_Color(color),
    m_NormalPtr(nullptr),
    m_W(0.0)
{
}

/* Constructors with function value */
Vertex::Vertex(const vec3f pos, float w):
    vec3f(pos),
    m_Color(vec3f(.6f, .6f, .6f)),
    m_NormalPtr(nullptr),
    m_W(w)
{
}

Vertex::Vertex(const float x, const float y, const float z, float w):
    vec3f(x, y, z),
    m_Color(vec3f(.6, .6, .6)),
    m_NormalPtr(nullptr),
    m_W(w)
{
}

Vertex::Vertex(const float x, const float y, const float z, const vec3f color, float w):
    vec3f(x, y, z),
    m_Color(color),
    m_NormalPtr(nullptr),
    m_W(w)
{
}

Vertex::~Vertex()
{
     //dtor
}

void Vertex::draw(bool useAlpha) const{
    glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA);
    glPointSize(3);
	float alpha = 1;
	if(useAlpha) {
		alpha = 1-(m_W);
		if(alpha<0)
			alpha = 0;
		if(alpha>1)
			alpha = 1;
		if(m_W>1)
		alpha = 0;
		float cl = m_W * 100;
		if(m_W>0)
			return;
		else
			glColor4f(1,-cl,0,1);
	}else{
		glColor4f(m_Color._v[0],m_Color._v[1],m_Color._v[2],alpha);
	}

    glBegin(GL_POINTS);
        glVertex3fv(_v);
    glEnd();
    glEnable(GL_LIGHTING);
}

void Vertex::highlight(const vec3f color){
    m_Color = color;
}

void Vertex::resetColor(){
    m_Color = vec3f(.6, .6, .6);
}
