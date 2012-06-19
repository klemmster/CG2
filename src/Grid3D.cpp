//
//  Grid3D.cpp
//  PointMesh
//
//  Created by Christopher Sierigk on 14.06.12.
//  Copyright (c) 2012 Smart Mobile Factory. All rights reserved.
//

#ifdef __APPLE__
  #include <GL/glew.h>
  #include <GLUT/glut.h>
#else
  #include <GL/glew.h>
  #include <GL/glut.h>
#endif

#include "Grid3D.hpp"
#include <math.h>
#include <iomanip>

using namespace Eigen;

Grid3D::Grid3D(KDTree tree, const size_t dim_x, const size_t dim_y, const size_t dim_z)
    : m_tree(tree), m_dimX(dim_x), m_dimY(dim_y), m_dimZ(dim_z)
{
    m_radius = 100.0f;
	m_interpolate = true;

    const VertexList& minm_vertices = m_tree.getMinVertices();
    const VertexList& maxm_vertices = m_tree.getMaxVertices();

    // slightly bigger bounding box
    m_MinX = (*minm_vertices.at(0))[0] - 0.01;
    m_MinY = (*minm_vertices.at(1))[1] - 0.01;
    m_MinZ = (*minm_vertices.at(2))[2] - 0.01;

    m_MaxX = (*maxm_vertices.at(0))[0] + 0.01;
    m_MaxY = (*maxm_vertices.at(1))[1] + 0.01;
    m_MaxZ = (*maxm_vertices.at(2))[2] + 0.01;

    m_diagLength = sqrt(pow((m_MaxX-m_MinX), 2) +
                        pow((m_MaxY-m_MinY),2) +
                        pow((m_MaxZ - m_MinZ),2));

    float stepX = abs(m_MaxX - m_MinX) / dim_x;
    float stepY = abs(m_MaxY - m_MinY) / dim_y;
    float stepZ = abs(m_MaxZ - m_MinZ) / dim_z;

    float xPos = m_MinX;
    float yPos = m_MinY;
    float zPos = m_MinZ;

    std::cout << "xPos: " << xPos << "\n";
    std::cout << "yPos: " << yPos << "\n";
    std::cout << "zPos: " << zPos << "\n";

    vec3f color(0.2, 0.8, 0.5);

    for (size_t z = 0; z <= m_dimZ; z++)
    {
        for (size_t y = 0; y <= m_dimY; y++)
        {
            for (size_t x = 0; x <= m_dimX; x++)
            {
                VertexPtr vrtx = VertexPtr(new Vertex(xPos, yPos, zPos, color, 0));
                //vrtx->highlight();
                m_GridVertices.push_back(vrtx);
                xPos += stepX;
            }

            yPos += stepY;
            xPos = m_MinX;
        }

        zPos += stepZ;
        yPos = m_MinY;
    }

    generateVertices();
    approximateWLS(m_GridVertices);
 }

void Grid3D::generateVertices()
    /* Create 2 more Points per point */
{
    for(VertexPtr vertex: m_tree.getVertices()){
        double epsilon = 0.1 * m_diagLength;if(false)
        while(m_tree.findInRadius(vertex, epsilon).size() > 0){
            //std::cout << "Size: " << m_tree.findInRadius(vertex, epsilon).size()<< "\n";
            epsilon /= 2.0;
            //std::cout << "Epsilon: " << epsilon << "\n";
        }
        VertexPtr point1(new Vertex((*vertex) + (*vertex->getNormal()), epsilon));
        VertexPtr point2(new Vertex((*vertex) - (*vertex->getNormal()), -epsilon));
        m_generatedPoints.push_back(point1);
        m_generatedPoints.push_back(point2);
    }

    VertexList allPoints(m_generatedPoints);
    for(auto vrtx: m_GridVertices){
        allPoints.push_back(vrtx);
    }
    m_fullTree = KDTree(allPoints, 3);
    approximateWLS(m_GridVertices);
}

void Grid3D::approximateWLS(VertexList& resultList)
{
    int m = 0;
    int d = 3;

    //int k = factorial(d + m) / (factorial(m) * factorial(d));
    int k = 1;
    std::cout << "K: " << k << "\n";
    VectorXf b(1);
    b << 1.0f;

    //Single Approximation for every Grid point
    for(VertexPtr pointDesired: resultList)
    {
        //Get Points used for this approximation
        //TODO: Should be a good automatic? radius -- maybe gridsize related
        VertexList list = m_fullTree.findInRadius(pointDesired, m_radius);
        MatrixXf bDimsMatSum = MatrixXf::Zero(k,k);
        VectorXf bDimsVecSum = VectorXf::Zero(k);

        for(VertexPtr point : list)
        {
            double funValue = point->getFunValue();
            Eigen::MatrixXf bDims(k,k);
            bDims = b*b.transpose();
            vec3f distVec = (*pointDesired) - (*point);
            float dist = norm(distVec);
            float wendFac = getWendland(dist);
            bDimsMatSum += (wendFac * bDims);
            bDimsVecSum += (wendFac * b*funValue);
        }

        //float x = (*pointDesired)[0];
        //float y = (*pointDesired)[1];
        //b << 1, x, y, x*x, x*y, y*y;

        // constant polynom basis
        /**
        VectorXf b(1);
        float x = (*pointDesired)[0];
        float y = (*pointDesired)[1];
        float z = (*pointDesired)[2];
        b << 1;
        **/

        // linear polynom basis
        /**
        VectorXf b(4);
        float x = (*pointDesired)[0];
        float y = (*pointDesired)[1];
        float z = (*pointDesired)[2];
        b << 1, x, y, z;
        **/

        // quadratic polynom basis
        /**
        VectorXf b(10);
        float x = (*pointDesired)[0];
        float y = (*pointDesired)[1];
        float z = (*pointDesired)[2];
        b << 1, x, y, z, x*x, x*y, x*z, y*y, y*z, z*z;
        **/

        VectorXf c(k);
        c = bDimsMatSum.inverse() * bDimsVecSum;
        double funValue = b.dot(c);
        pointDesired->setFunValue(funValue);
        if(funValue < 0.0f){
            pointDesired->highlight(vec3f(1.0f, 1.0f, 1.0f));
            std::cout << "Highlight\n";
        }
        //m_coefficients.push_back(c);
    }
}

void Grid3D::draw()
{
    for (VertexPtr vrtx : m_GridVertices)
    {
        vrtx->draw();
    }
}

unsigned int Grid3D::factorial(const int num)
{
    unsigned int result = 1;

    for (int i = 1; i <= num; ++i)
    {
        result *= i;
    }

    return result;
}


//--- Getting values ---

int Grid3D::getIndex(int idX,int idY,int idZ) {
	if(idX<0 || idX>=m_dimX || idY<0 || idY>=m_dimY || idZ<0 || idZ>=m_dimZ)
		return -1; 
	return idZ*m_dimX*m_dimY + idY*m_dimX + idX;
}

shared_ptr<Vertex> Grid3D::getVertex(int idX,int idY,int idZ) {
	int index = getIndex(idX,idY,idZ);
	if(index<0)
		return NULL;
	else
		return m_GridVertices.at(index);
}

double Grid3D::getVertexValue(int idX,int idY,int idZ) {
	shared_ptr<Vertex> vertex = getVertex(idX,idY,idZ);
	if(vertex==NULL)
		return OUTOFRANGE_DISTANCE;
	else
		return vertex->getFunValue();
}


double Grid3D::getInterpolatedFunctionValue(float x,float y,float z) {
	if(m_interpolate) {
		int iX = (int)x;
		int iY = (int)y;
		int iZ = (int)z;
		float u = x-iX;
		float v = y-iY;
		float w = z-iZ;
		float uR = 1-u;
		float vR = 1-v;
		float wR = 1-w;
		float sum = 0;
		sum += getVertexValue(iX,iY,iZ) * uR*vR*wR;
		sum += getVertexValue(iX+1,iY,iZ) * u*vR*wR;
		sum += getVertexValue(iX,iY+1,iZ) * uR*v*wR;
		sum += getVertexValue(iX+1,iY+1,iZ) * u*v*wR;
		sum += getVertexValue(iX,iY,iZ+1) * uR*vR*w;
		sum += getVertexValue(iX+1,iY,iZ+1) * u*vR*w;
		sum += getVertexValue(iX,iY+1,iZ+1) * uR*v*w;
		sum += getVertexValue(iX+1,iY+1,iZ+1) * u*v*w;
		return sum;
	}else{
		return getVertexValue((int)x,(int)y,(int)z);
	}
}

double Grid3D::getImplicitFunctionValueWorldCoordinates(float x,float y,float z) {
	//if(x<m_MinX || x>m_MaxX || y<m_MinY || y>m_MaxY || z<m_MinZ || z>m_MaxZ)
	//	return OUTOFRANGE_DISTANCE;
	//else
	//	return -0.5f;
	x = (x - m_MinX)/(m_MaxX-m_MinX) * (m_dimX-1);
	y = (y - m_MinY)/(m_MaxY-m_MinY) * (m_dimY-1);
	z = (z - m_MinZ)/(m_MaxZ-m_MinZ) * (m_dimZ-1);
	return getInterpolatedFunctionValue(x,y,z);
	
//return x*x*4 + y*y*6 + z*z*8 - 1;
}
