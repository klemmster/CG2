#ifndef HYPERSPHERE_HPP_KRFJMWVH
#define HYPERSPHERE_HPP_KRFJMWVH

#include "hyperRegion.hpp"
#include "vec.hpp"

class HyperSphere: public HyperRegion
{
public:
    HyperSphere (const VertexPtr& source, const float radius);
    virtual ~HyperSphere ();

    virtual bool inRegion(const VertexPtr src) const;
    virtual bool intersectsRegion(const Boundaries boundaries) const;

private:

    const float m_radius;
    const float m_radius_sqr;
    VertexPtr m_source;
    vec3f m_min;
    vec3f m_max;
};

#endif /* end of include guard: HYPERSPHERE_HPP_KRFJMWVH */

