#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

#include "Grid.hpp"
#include "vertex.hpp"
#include <math.h>

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;


Grid::Grid (KDTree tree, const size_t dim_x, const size_t dim_y):
    m_tree(tree), m_dimX(dim_x), m_dimY(dim_y)
{

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
    approximateWLS();
}

void Grid::approximateLS(){
    MatrixXf bDimsMatSum(6,6);
    VectorXf bDimsVecSum(6);

    VertexPtr midPoint(new Vertex(0.0, 0.0, 0.0));
    VertexList list = m_tree.findInRadius(midPoint, 40000);

    for(VertexPtr point : list){
        VectorXf b(6);
        float x = (*point)[0];
        float y = (*point)[1];
        float z = (*point)[2];
        b << 1, x, y, x*x, x*y, y*y;
        MatrixXf bDims(6,6);
        bDims = b*b.transpose();
        bDimsMatSum += bDims;
        bDimsVecSum += (b*z);
    }

    VectorXf c(6);
    c = bDimsMatSum.inverse() * bDimsVecSum;

    for(VertexPtr point : m_vertices){
        VectorXf b(6);
        float x = (*point)[0];
        float y = (*point)[1];
        b << 1, x, y, x*x, x*y, y*y;
        (*point)[2] = b.dot(c);
    }
}

void Grid::approximateWLS(){

    //Single Approximation for every Grid point
    for(VertexPtr pointDesired: m_vertices){
        //Get Points used for this approximation
        //TODO: Should be a good automatic? radius -- maybe gridsize related
        VertexList list = m_tree.findInRadius(pointDesired, .3);
        MatrixXf bDimsMatSum(6,6);
        VectorXf bDimsVecSum(6);
        for(VertexPtr point : list){
            VectorXf b(6);
            float x = (*point)[0];
            float y = (*point)[1];
            float z = (*point)[2];
            b << 1, x, y, x*x, x*y, y*y;
            MatrixXf bDims(6,6);
            bDims = b*b.transpose();
            vec3f distVec = (*pointDesired) - (*point);
            float dist = norm(distVec);
            float wendFac = getWendland(dist);
            bDimsMatSum += (wendFac * bDims);
            bDimsVecSum += (wendFac * b*z);
        }
       VectorXf b(6);
       float x = (*pointDesired)[0];
       float y = (*pointDesired)[1];
       b << 1, x, y, x*x, x*y, y*y;

       VectorXf c(6);
       c = bDimsMatSum.inverse() * bDimsVecSum;
       (*pointDesired)[2] = b.dot(c);
    }
}

float Grid::getWendland(const float distance) const{
    //(1 - d/h)^4 * (4d/h +1)
    // term1      *   term2
    float h = 0.01f;
    float term1Part = (1.0f - distance/h);
    float term1 = std::pow(term1Part, 4);
    float term2 = (4*distance/h)+1;
    return term1 * term2;
}


void Grid::draw(){

    glBegin(GL_LINE_STRIP);
        glVertex3f(m_MinX, m_MinY, m_MinZ);
        glVertex3f(m_MaxX, m_MinY, m_MinZ);
        glVertex3f(m_MaxX, m_MaxY, m_MinZ);
        glVertex3f(m_MinX, m_MaxY, m_MinZ);
        glVertex3f(m_MinX, m_MinY, m_MinZ);
    glEnd();

    for(size_t i = 0; i < m_vertices.size() - m_dimX - 1; i++)
    {

        if ((i + 1) % m_dimX == 0)
        {
            continue;
        }

        VertexPtr vec1 = m_vertices.at(i + 0);
        VertexPtr vec2 = m_vertices.at(i + 1);
        VertexPtr vec3 = m_vertices.at(i + m_dimX + 0);
        VertexPtr vec4 = m_vertices.at(i + m_dimX + 1);

        vec3f v1(vec1->_v[0], vec1->_v[1], vec1->_v[2]);
        vec3f v2(vec2->_v[0], vec2->_v[1], vec2->_v[2]);
        vec3f v3(vec3->_v[0], vec3->_v[1], vec3->_v[2]);
        vec3f v4(vec4->_v[0], vec4->_v[1], vec4->_v[2]);

        vec3f normal_v1_v2 = normalize(cross(v2, v1));
        vec3f normal_v3_v2 = normalize(cross(v2, v3));

        //float colorBlue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
        //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorBlue);

        glColor3f(0, 0, 1);
        glBegin(GL_TRIANGLES);
            glVertex3fv(vec1->_v);                  // 0    // 2
            glNormal3fv(normal_v1_v2._v);
            glVertex3fv(vec2->_v);                  // 1    // 3
            glNormal3fv(normal_v1_v2._v);
            glVertex3fv(vec3->_v);                  // 16   // 18
            glNormal3fv(normal_v1_v2._v);

            glVertex3fv(vec3->_v);
            glNormal3fv(normal_v3_v2._v);
            glVertex3fv(vec2->_v);
            glNormal3fv(normal_v3_v2._v);
            glVertex3fv(vec4->_v);
            glNormal3fv(normal_v3_v2._v);

        glEnd();

        glColor3f(0, 1, 0);
        glBegin(GL_LINES);

            glVertex3fv(vec1->_v);
            glVertex3fv(((v1 + (normal_v1_v2 / 10)))._v);
            glVertex3fv(vec2->_v);
            glVertex3fv(((v2 + (normal_v1_v2 / 10)))._v);
            glVertex3fv(vec3->_v);
            glVertex3fv(((v3 + (normal_v1_v2 / 10)))._v);

            glVertex3fv(vec3->_v);
            glVertex3fv(((v3 + (normal_v3_v2 / 10)))._v);
            glVertex3fv(vec2->_v);
            glVertex3fv(((v2 + (normal_v3_v2 / 10)))._v);
            glVertex3fv(vec4->_v);
            glVertex3fv(((v4 + (normal_v3_v2 / 10)))._v);

        glEnd();

    }
    glEnd();

    glColor3f(0, 1, 0);
    glBegin(GL_POINTS);
    for(VertexPtr vrtx: m_vertices){
        vrtx->draw();
    }
    glEnd();
}

