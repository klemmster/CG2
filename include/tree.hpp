#ifndef TREE_HPP_HE0Q8ZRG
#define TREE_HPP_HE0Q8ZRG

#include "vertex.hpp"

class KDTree
{
public:
    KDTree (const VertexList vertices);

    void collectKNearest() {throw "NOT yet implemented";};
    void collectInRadius() {throw "NOT yet implemented";};
    void draw() { throw "NOT yet implemented"; };
    void draw(const VertexList vertices) { throw "NOT yet implemented";};

    virtual ~KDTree ();

private:
    /* data */
};

#endif /* end of include guard: TREE_HPP_HE0Q8ZRG */

