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
#include <float.h>
#include <iomanip>

#define DIM_BIAS 0.2f

using namespace Eigen;

#include <iomanip>
Grid3D::Grid3D(KDTree tree, const size_t dim_x, const size_t dim_y, const size_t dim_z, const float radius):
    Grid(tree, dim_x, dim_y),
    m_dimZ(dim_z)
{
    m_radius = radius;
    std::cout << "Radius: " << std::setprecision(5) << m_radius << "\n";
	m_interpolate = true;

    const VertexList& minm_vertices = m_tree.getMinVertices();
    const VertexList& maxm_vertices = m_tree.getMaxVertices();

    // slightly bigger bounding box
    m_MinX = (*minm_vertices.at(0))[0] - DIM_BIAS;
    m_MinY = (*minm_vertices.at(1))[1] - DIM_BIAS;
    m_MinZ = (*minm_vertices.at(2))[2] - DIM_BIAS;

    m_MaxX = (*maxm_vertices.at(0))[0] + DIM_BIAS;
    m_MaxY = (*maxm_vertices.at(1))[1] + DIM_BIAS;
    m_MaxZ = (*maxm_vertices.at(2))[2] + DIM_BIAS;

    m_diagLength = sqrt(pow((m_MaxX-m_MinX), 2) +
                        pow((m_MaxY-m_MinY),2) +
                        pow((m_MaxZ - m_MinZ),2));

    float stepX = abs(m_MaxX - m_MinX) / dim_x;
    float stepY = abs(m_MaxY - m_MinY) / dim_y;
    float stepZ = abs(m_MaxZ - m_MinZ) / dim_z;

    //std::cout << "StepX: " << std::setprecision(5) << stepX << "\n";
    std::cout << "Diagonal: " << std::setprecision(5) << m_diagLength << "\n";

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
                VertexPtr vrtx = VertexPtr(new Vertex(xPos, yPos, zPos, color, FLT_MAX));
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
        float epsilon = 0.01 * m_diagLength;
        while(m_tree.findInRadius(vertex, epsilon).size() > 1){
            epsilon /= 1.1;
        }
        NormalPtr normal = vertex->getNormal();
        (*normal) = ((*normal) * epsilon);
        VertexPtr point1(new Vertex((*vertex) + (*normal), epsilon));
        VertexPtr point2(new Vertex((*vertex) - (*normal), -epsilon));
        m_generatedPoints.push_back(point1);
        m_generatedPoints.push_back(point2);
        m_generatedPoints.push_back(vertex);
    }

    m_fullTree = KDTree(m_generatedPoints, 3);
    approximateWLS(m_GridVertices);
}

void Grid3D::approximateWLS(VertexList& resultList)
{
    int m = 0;
    int d = 3;

    //int k = factorial(d + m) / (factorial(m) * factorial(d));
    int k = 1;
    std::cout << "K: " << k << "\n";
    VectorXf b = VectorXf::Zero(1);
    b << 1.0f;

    //Single Approximation for every Grid point
    for(VertexPtr pointDesired: resultList)
    {
        //Get Points used for this approximation
        VertexList list = m_fullTree.findInRadius(pointDesired, m_radius);
        MatrixXf bDimsMatSum = MatrixXf::Zero(k,k);
        VectorXf bDimsVecSum = VectorXf::Zero(1);

        //std::cout << "Size: " << list.size() << "\n";
        if(list.size() > 0){
            for(VertexPtr point : list)
            {
                float funValue = point->getFunValue();
                //std::cout << "funvalue: " << std::setprecision(5) << funValue << "\n";
                Eigen::MatrixXf bDims = MatrixXf::Zero(k,k);
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

            VectorXf c = VectorXf::Zero(k);
            c = bDimsMatSum.inverse() * bDimsVecSum;
            float funValue = b.dot(c);
            NormalPtr normal = interpolateNormal(list);
            pointDesired->setFunValue(funValue);
            pointDesired->setNormal(normal);
            if(std::isinf(funValue) != 0 || std::isnan(funValue) != 0){
                std::cout << "Bad FunValue\n";
            }
            if(funValue < 0.0f){
                pointDesired->highlight(vec3f(1.0f, 0.0f, 0.0f));
            }
        }
            //m_coefficients.push_back(c);
    }
}

NormalPtr Grid3D::interpolateNormal(VertexList vrtxList) const{
    std::vector<NormalPtr> normals;
    for(VertexPtr vrtx : vrtxList){
        NormalPtr normal = vrtx->getNormal();
        if(normal){
            (*normal) = (*normal) / norm(*normal);
            normals.push_back(normal);
        }
    }
    //TODO: distance - dependent interpolation if necessary
    float factor = 1.0/normals.size();
    vec3f avgNormal;
    for(NormalPtr normal : normals){
        avgNormal = ((*normal)*factor)+avgNormal;
    }
    return NormalPtr(new Normal(avgNormal));
}

void Grid3D::draw(bool useAlpha)
{
    for (VertexPtr vrtx : m_GridVertices)
    {
        vrtx->draw(useAlpha);
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
	if(idX<0 || idX>m_dimX || idY<0 || idY>m_dimY || idZ<0 || idZ>m_dimZ)
		return -1;
	return idZ*(m_dimX+1)*(m_dimY+1) + idY*(m_dimX+1) + idX;
}

shared_ptr<Vertex> Grid3D::getVertex(int idX,int idY,int idZ) {
	int index = getIndex(idX,idY,idZ);
	if(index<0)
		return nullptr;
	else
		return m_GridVertices.at(index);
}

float Grid3D::getVertexValue(int idX,int idY,int idZ,int valueType) {
	shared_ptr<Vertex> vertex = getVertex(idX,idY,idZ);
	if(vertex==nullptr){
		return OUTOFRANGE_DISTANCE;
    }else{
		if(valueType == 0){
			return vertex->getFunValue();
        }else{
			if(vertex->getNormal()!=nullptr)
				return (*vertex->getNormal())[0];
			else
				return 0;
		}
	}
}


float Grid3D::getInterpolatedFunctionValue(float x,float y,float z,int valueType) {
	if(m_interpolate) {
		int iX = (int)x;
		int iY = (int)y;
		int iZ = (int)z;
        if(iX >= m_dimX || iY >= m_dimY || iZ >= m_dimZ){
            return FLT_MAX;
        }
		float u = x-iX;
		float v = y-iY;
		float w = z-iZ;
		float uR = 1-u;
		float vR = 1-v;
		float wR = 1-w;
		float sum = 0;
        double val = getVertexValue(iX,iY,iZ,valueType);
        if(val != FLT_MAX)
		    sum +=  val * uR*vR*wR;
        val = getVertexValue(iX+1,iY,iZ,valueType);
        if(val != FLT_MAX)
            sum += val * u*vR*wR;
        val = getVertexValue(iX,iY+1,iZ,valueType);
        if(val != FLT_MAX)
		    sum += val * uR*v*wR;
        val = getVertexValue(iX+1,iY+1,iZ,valueType);
        if(val != FLT_MAX)
            sum += val * u*v*wR;
        val = getVertexValue(iX,iY,iZ+1,valueType);
        if(val != FLT_MAX)
            sum += val * uR*vR*w;
        val = getVertexValue(iX+1,iY,iZ+1,valueType);
        if(val != FLT_MAX)
            sum += val * u*vR*w;
        val = getVertexValue(iX,iY+1,iZ+1,valueType);
        if(val != FLT_MAX)
            sum +=  val * uR*v*w;
        val = getVertexValue(iX+1,iY+1,iZ+1,valueType);
        if(val != FLT_MAX)
            sum += val * u*v*w;
		return sum;
	}else{
		return getVertexValue((int)x,(int)y,(int)z,valueType);
	}
}

float Grid3D::getImplicitFunctionValueWorldCoordinates(float x,float y,float z,int valueType) {
	//if(x<m_MinX || x>m_MaxX || y<m_MinY || y>m_MaxY || z<m_MinZ || z>m_MaxZ)
	//	return OUTOFRANGE_DISTANCE;
	//else
	//	return -0.5f;
	x = (x - m_MinX)/(m_MaxX-m_MinX) * (m_dimX);
	y = (y - m_MinY)/(m_MaxY-m_MinY) * (m_dimY);
	z = (z - m_MinZ)/(m_MaxZ-m_MinZ) * (m_dimZ);
    float val = getInterpolatedFunctionValue(x,y,z,valueType);
    if(std::isnan(val)!= 0 || std::isinf(val)!= 0){
        std::cout << "Interpolatd Value: " << val << "\n";
        std::cout << "X: " << x << " Y: " << y << " Z: " << z << "\n";
        return 0.0f;
    }
	return val;
//return x*x*4 + y*y*6 + z*z*8 - 1;
}
