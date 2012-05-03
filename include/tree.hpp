#ifndef TREE_HPP_HE0Q8ZRG
#define TREE_HPP_HE0Q8ZRG

#include "vertex.hpp"
#include "leaf.hpp"

typedef std::tuple<VertexList, VertexList,VertexList> ListTriple;
typedef std::pair<VertexList, VertexList> ListPair;
class KDTree
{
public:
    KDTree (const VertexList vertices);

    void collectKNearest() {throw "NOT yet implemented";};
    void collectInRadius() {throw "NOT yet implemented";};
    void draw() { throw "NOT yet implemented"; };
    void draw(const VertexList vertices) { throw "NOT yet implemented";};

    VertexList findNearestNeighbour(const VertexPtr source);
    VertexList findInRadius(const VertexPtr source, const size_t radius);

    virtual ~KDTree ();

private:

    LeafPtr makeTree(size_t depth, ListTriple t);

    ListPair splitListBy(const size_t index, const VertexList sourceList,
            const VertexPtr sourceVert);

    LeafPtr m_root;

};

#endif /* end of include guard: TREE_HPP_HE0Q8ZRG */

