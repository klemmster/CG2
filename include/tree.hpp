#ifndef TREE_HPP_HE0Q8ZRG
#define TREE_HPP_HE0Q8ZRG

#include <array>

#include "vertex.hpp"
#include "node.hpp"
#include "hyperSphere.hpp"

typedef std::array<VertexList, 3> ListTriple;
typedef std::pair<VertexList, VertexList> ListPair;
class KDTree
{
public:
    KDTree (const VertexList vertices);

    void draw() { throw "NOT yet implemented"; };
    void draw(const VertexList vertices) { throw "NOT yet implemented";};

    VertexList findNearestNeighbour(const VertexPtr source);
    VertexList findInRadius(const VertexPtr source, const size_t radius);

    virtual ~KDTree ();

private:

    NodePtr makeTree(size_t depth, ListTriple t, Boundaries boundaries);

    ListPair splitListBy(const size_t index, const VertexList sourceList,
            const VertexPtr sourceVert);

    void findInRadius(const NodePtr& src, const HyperSphere& sphere,
            VertexList& result) const;

    NodePtr m_root;

};

#endif /* end of include guard: TREE_HPP_HE0Q8ZRG */

