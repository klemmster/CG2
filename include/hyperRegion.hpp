#ifndef HYPERREGION_HPP_SV8YTUQ2
#define HYPERREGION_HPP_SV8YTUQ2

#include "vertex.hpp"

class HyperRegion
{
public:
    HyperRegion ();
    virtual ~HyperRegion ();

    virtual bool inRegion(const VertexPtr srcVertex) = 0;
    virtual bool intersectsRegion(const

private:
    /* data */
};


#endif /* end of include guard: HYPERREGION_HPP_SV8YTUQ2 */

