#include "hyperSphere.hpp"
#include <math.h>

HyperSphere::HyperSphere(const VertexPtr& source, const float radius):
        m_radius(radius),
        m_radius_sqr(radius*radius),
        m_source(source){
        m_min = vec3f((*source)[0]-radius, (*source)[1]-radius, (*source)[2]-radius);
        m_max = vec3f((*source)[0]+radius, (*source)[1]+radius, (*source)[2]+radius);

};

HyperSphere::~HyperSphere() {};

bool HyperSphere::contains(const VertexPtr src) const{
    Vertex tmp = (*src) - (*m_source);
    float dist = norm(tmp);
    return (dist < m_radius);
}

bool HyperSphere::intersectsRegion(const Domain& domain) const{
    float dmin = 0;
    for( size_t i; i<3; i++){
        if( (*m_source)[i] < m_min[i]){
            dmin += sqrtf((*m_source)[i] - m_min[i] );
        }else if( (*m_source)[i] > m_max[i]){
            dmin += sqrtf((*m_source)[i] - m_max[i] );
        }
    }
    return dmin <= m_radius_sqr;
}

bool HyperSphere::withinRegion(const Domain& domain) const{
    if(m_min[0] > domain.getMin(0) && m_max[0] < domain.getMax(0) &&
       m_min[1] > domain.getMin(1) && m_max[1] < domain.getMax(1) &&
       m_min[2] > domain.getMin(2) && m_max[2] < domain.getMax(2)){
        return true;
    }
    return false;
}

