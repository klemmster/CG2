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

typedef std::vector< VertexList > VertexLists;
typedef std::pair<VertexList, VertexList> ListPair;

class KDTree
{
public:
    KDTree ();
    KDTree (const VertexList vertices, const size_t dimensions);

    void draw();
    void draw(const VertexList vertices);

    VertexList findKNearestNeighbours(const VertexPtr source, const size_t numNeighbours);
    VertexList findInRadius(const VertexPtr source, const float radius);
    const VertexList& getVertices() const{ return m_origVertices; };
    const VertexList& getMinVertices() const;
    const VertexList& getMaxVertices() const;

    virtual ~KDTree ();

private:

    void drawSingleNode(const NodePtr& src);

    NodePtr makeTree(size_t depth, const size_t& cellSize, VertexLists& t,
            const Domain& domain);

    ListPair splitListBy(const size_t& index, const VertexList& sourceList,
            const VertexPtr& sourceVert);

    void findInRadius(const NodePtr& src, const HyperSphere& sphere,
            VertexList& result) const;

    void findKNearestNeighbours(const NodePtr& src, LimitedPriorityQueue& results,
            const VertexPtr& target);


    NodePtr m_root;
    VertexList m_MinVertices;
    VertexList m_MaxVertices;
    VertexList m_origVertices;
    size_t m_K;
};

typedef NodePtr(KDTree::*makeTreeFun)(size_t depth, const size_t& cellSize, VertexLists& t,
            const Domain& domain);

#endif /* end of include guard: TREE_HPP_HE0Q8ZRG */

