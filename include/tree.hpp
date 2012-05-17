#ifndef TREE_HPP_HE0Q8ZRG
#define TREE_HPP_HE0Q8ZRG

#include <array>

#include "vertex.hpp"
#include "node.hpp"
#include "hyperSphere.hpp"
#include "limitedPriorityQueue.hpp"

#ifdef __APPLE__
#include <boost/thread/future.hpp>
#include <boost/thread/thread.hpp>
#else
#include <future>
#include <thread>
#endif

typedef std::array<VertexList, 3> ListTriple;
typedef std::pair<VertexList, VertexList> ListPair;
class KDTree
{
public:
    KDTree ();
    KDTree (const VertexList vertices);

    void draw();
    void draw(const VertexList vertices);

    VertexList findKNearestNeighbours(const VertexPtr source, const size_t numNeighbours);
    VertexList findInRadius(const VertexPtr source, const size_t radius);
    const VertexList& getMinVertices() const;
    const VertexList& getMaxVertices() const;

    virtual ~KDTree ();

private:

    void drawSingleNode(const NodePtr& src);

    NodePtr makeTree(size_t depth, const size_t& cellSize, ListTriple& t,
            const Boundaries& boundaries);

    ListPair splitListBy(const size_t& index, const VertexList& sourceList,
            const VertexPtr& sourceVert);

    void findInRadius(const NodePtr& src, const HyperSphere& sphere,
            VertexList& result) const;

    void findKNearestNeighbours(const NodePtr& src, LimitedPriorityQueue& results,
            const VertexPtr& target);


    NodePtr m_root;
    VertexList m_MinVertices;
    VertexList m_MaxVertices;
};

typedef NodePtr(KDTree::*makeTreeFun)(size_t depth, const size_t& cellSize, ListTriple& t,
            const Boundaries& boundaries);

#endif /* end of include guard: TREE_HPP_HE0Q8ZRG */

