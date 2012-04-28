#include "tree.hpp"

#include <algorithm>
KDTree::KDTree(const VertexList vertices){

    // Copy Input arrays for individual sorting
    VertexList sortedY(vertices);
    VertexList sortedX(vertices);
    VertexList sortedZ(vertices);

    std::sort(sortedX.begin(), sortedX.end(), Vertex::sortX );
    std::sort(sortedY.begin(), sortedY.end(), Vertex::sortY );
    std::sort(sortedZ.begin(), sortedZ.end(), Vertex::sortZ );

}

KDTree::~KDTree(){

}

