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

bool HyperSphere::intersectsRegion(const Boundaries& boundaries) const{
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

bool HyperSphere::withinRegion(const Boundaries& boundaries) const{
    if(m_min[0] > boundaries[0] && m_max[0] < boundaries[1] &&
       m_min[1] > boundaries[2] && m_max[1] < boundaries[3] &&
       m_min[2] > boundaries[4] && m_max[2] < boundaries[5]){
        return true;
    }
    return false;
}

