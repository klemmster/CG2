#include "hyperSphere.hpp"
#include <math.h>

HyperSphere::HyperSphere(const VertexPtr& source, const float radius, const size_t dim):
        m_radius(radius),
        m_radius_sqr(radius*radius),
        m_source(source),
        m_dim(dim){
        m_min = vec3f((*source)[0]-radius, (*source)[1]-radius, (*source)[2]-radius);
        m_max = vec3f((*source)[0]+radius, (*source)[1]+radius, (*source)[2]+radius);
};

HyperSphere::~HyperSphere() {};

bool HyperSphere::contains(const VertexPtr src) const{
    Vertex tmp = (*src) - (*m_source);
    float distSqrt = 0.0;
    for(size_t i = 0; i<m_dim; ++i){
        distSqrt += (tmp[i] * tmp[i]);
    }
    float dist = std::sqrt(distSqrt);
    return (dist < m_radius);
}

bool HyperSphere::intersectsRegion(const Domain& domain) const{
    float dmin = 0;
    for( size_t i; i<m_dim; i++){
        if( (*m_source)[i] < m_min[i]){
            dmin += sqrtf((*m_source)[i] - m_min[i] );
        }else if( (*m_source)[i] > m_max[i]){
            dmin += sqrtf((*m_source)[i] - m_max[i] );
        }
    }
    return dmin <= m_radius_sqr;
}

bool HyperSphere::withinRegion(const Domain& domain) const{
    for(size_t i=0; i<m_dim; ++i){
        if(!(m_min[i] > domain.getMin(i) && m_max[0] < domain.getMax(i))){
            return false;
        }
    }
    return true;
}

