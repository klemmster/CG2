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


using namespace Eigen;


Grid::Grid (KDTree tree, const size_t dim_x, const size_t dim_y):
    m_tree(tree), m_dimX(dim_x), m_dimY(dim_y), m_showQuads(false)
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
    //TODO Use GUI
    repeatedApproximation(2);
    toggleQuads();
}

void Grid::toggleQuads(){
    m_showQuads =  !m_showQuads;
}

void Grid::approximateLS(VertexList & resultList){
    MatrixXf bDimsMatSum(6,6);
    VectorXf bDimsVecSum(6);

    bDimsMatSum <<  0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0;
    bDimsVecSum << 0, 0, 0, 0, 0, 0;



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

    for(VertexPtr point : resultList){
        VectorXf b(6);
        float x = (*point)[0];
        float y = (*point)[1];
        b << 1, x, y, x*x, x*y, y*y;
        (*point)[2] = b.dot(c);
    }
}

void Grid::approximateWLS(VertexList& resultList){
    //Single Approximation for every Grid point
    for(VertexPtr pointDesired: resultList){
        //Get Points used for this approximation
        //TODO: Should be a good automatic? radius -- maybe gridsize related
        VertexList list = m_tree.findInRadius(pointDesired, .3);
        MatrixXf bDimsMatSum(6,6);
        VectorXf bDimsVecSum(6);
        bDimsMatSum <<  0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0;
        bDimsVecSum << 0, 0, 0, 0, 0, 0;

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
       m_coefficients.push_back(c);
    }
}

void Grid::approximateTensor(const size_t k){

    m_k = k;
}

void Grid::repeatedApproximation(const size_t k){

    m_k = k;
    const VertexList& minm_vertices = m_tree.getMinVertices();
    const VertexList& maxm_vertices = m_tree.getMaxVertices();

    m_MinX = (*minm_vertices.at(0))[0];
    m_MinY = (*minm_vertices.at(1))[1];
    m_MinZ = (*minm_vertices.at(2))[2];

    m_MaxX = (*maxm_vertices.at(0))[0];
    m_MaxY = (*maxm_vertices.at(1))[1];
    m_MaxZ = (*maxm_vertices.at(2))[2];

    float xStep = abs(m_MaxX - m_MinX) / (m_dimX * k);
    float yStep = abs(m_MaxY - m_MinY) / (m_dimY * k);

    float xPos = m_MinX;
    float yPos = m_MinY;

    vec3f color(0.2, 0.8, 0.5);

    for(size_t y = 0; y < m_dimY*k; ++y){
        for(size_t x = 0; x < m_dimX*k; ++x){
            m_interpolVertices.push_back(VertexPtr(new Vertex(xPos, yPos, m_MaxZ, color)));
            xPos += xStep;
        }
            yPos += yStep;
            xPos = m_MinX;
    }

    approximateWLS(m_interpolVertices);
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
    if(m_showQuads){
        drawQuads();
    }else{
        drawTriangles();
    }
}

void Grid::drawQuads(){
    glBegin(GL_QUADS);
        size_t width = m_dimX * m_k;
        for(size_t x=0; x<width-1; ++x){
            for(size_t y=0; y<(m_dimY*m_k)-1; ++y){
                size_t topLeft = (y*width)+x;
                size_t topRight = (y*width)+x +1;
                size_t bottomLeft = ((y+1)*width) + x;
                size_t bottomRight = ((y+1)*width) + x + 1;
                
                VertexPtr ltVrtx = m_interpolVertices.at(topLeft);
                VertexPtr rtVrtx = m_interpolVertices.at(topRight);
                VertexPtr lbVrtx = m_interpolVertices.at(bottomLeft);
                VertexPtr rbVrtx = m_interpolVertices.at(bottomRight);
                
                vec3f normal = normalize( cross( (*rtVrtx) - (*ltVrtx), (*ltVrtx) - (*lbVrtx) ) );
                
                glNormal3fv(normal._v);
                glVertex3fv((*ltVrtx)._v);
                glNormal3fv(normal._v);
                glVertex3fv((*rtVrtx)._v);
                glNormal3fv(normal._v);
                glVertex3fv((*rbVrtx)._v);
                glNormal3fv(normal._v);
                glVertex3fv((*lbVrtx)._v);

                //TODO Normals
            }
        }
    glEnd();
}

void Grid::drawTriangles(){
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

        vec3f normal_v1_v2 = normalize(cross((*vec1) - (*vec3), (*vec2) - (*vec1)));
        vec3f normal_v3_v2 = normalize(cross((*vec3) - (*vec4), (*vec2) - (*vec3)));

        float colorGrey[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorGrey);

        glBegin(GL_TRIANGLES);
            glNormal3fv(normal_v1_v2._v);
            glVertex3fv(vec1->_v);
            glNormal3fv(normal_v1_v2._v);
            glVertex3fv(vec2->_v);
            glNormal3fv(normal_v1_v2._v);
            glVertex3fv(vec3->_v);

            glNormal3fv(normal_v3_v2._v);
            glVertex3fv(vec3->_v);
            glNormal3fv(normal_v3_v2._v);
            glVertex3fv(vec2->_v);
            glNormal3fv(normal_v3_v2._v);
            glVertex3fv(vec4->_v);

        glEnd();

        glDisable(GL_LIGHTING);

        //float colorGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
        //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorGreen);
        glColor3f(0, 1, 0);
        glBegin(GL_LINES);

            glVertex3fv((( (( (*vec1) + (*vec2) + (*vec3) ) / 3)))._v);
            glVertex3fv((( (( (*vec1) + (*vec2) + (*vec3) ) / 3) + (normal_v1_v2 / 10)))._v);

            glVertex3fv((( (( (*vec3) + (*vec2) + (*vec4) ) / 3)))._v);
            glVertex3fv((( (( (*vec3) + (*vec2) + (*vec4) ) / 3) + (normal_v3_v2 / 10)))._v);

        glEnd();

        glEnable(GL_LIGHTING);


    }

    glDisable(GL_LIGHTING);

   // float colorRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorRed);
    glColor3f(1, 1, 0);
    glBegin(GL_POINTS);
    for(VertexPtr vrtx: m_vertices){
        vrtx->draw();
    }
    glEnd();
    glEnable(GL_LIGHTING);

}

