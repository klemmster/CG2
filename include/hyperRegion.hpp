#ifndef HYPERREGION_HPP_SV8YTUQ2
#define HYPERREGION_HPP_SV8YTUQ2

#include "vertex.hpp"
#include "node.hpp"
#include "domain.hpp"

class HyperRegion
{
public:
    HyperRegion ();
    virtual ~HyperRegion ();

    /* Yields true if VertexPtr is within this region */
    virtual bool contains(const VertexPtr srcVertex) const = 0;

    /* Yields true if Region is within boundaries */
    virtual bool intersectsRegion(const Domain& domain) const = 0;

private:
    /* data */
};


#endif /* end of include guard: HYPERREGION_HPP_SV8YTUQ2 */

