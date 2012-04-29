#include "tree.hpp"

#include <algorithm>
KDTree::KDTree(const VertexList vertices){

    // Copy Input arrays for individual sorting
    VertexList sortedX(vertices);
    VertexList sortedY(vertices);
    VertexList sortedZ(vertices);

    std::sort(sortedX.begin(), sortedX.end(), Vertex::sortX );
    std::sort(sortedY.begin(), sortedY.end(), Vertex::sortY );
    std::sort(sortedZ.begin(), sortedZ.end(), Vertex::sortZ );
    ListTuple t(sortedX, sortedY, sortedZ);
    m_root = makeTree(0, t);
};

KDTree::~KDTree(){

};

LeafPtr KDTree::makeTree(size_t depth, ListTuple t){
    /*
     * Tuple contains x, y, z  Dimensions Vertex list
     *
    */
    // TODO: More sophisticated Implementation, taking other axis into account etc.
    VertexList xVertices = std::get<0>(t);

    std::cout << "Size: " << xVertices.size() << "\n";
    if(xVertices.size() == 0){
        return nullptr;
    }
    if(xVertices.size() == 1){
        return LeafPtr(new Leaf(xVertices.at(0), nullptr, nullptr));
    }
    size_t median = (int) (xVertices.size()-1)/2;
    VertexPtr posElement = xVertices.at(median);
    VertexList leftX = VertexList(xVertices.begin(), xVertices.begin()+median);
    VertexList leftY = std::get<1>(t);
    VertexList leftZ = std::get<2>(t);
    ListTuple leftTuple(leftX, leftY, leftZ);

    VertexList rightX= VertexList(xVertices.begin()+median+1, xVertices.end());
    VertexList rightY = std::get<1>(t);
    VertexList rightZ = std::get<2>(t);
    ListTuple rightTuple(rightX, rightY, rightZ);
    //return nullptr;
    return LeafPtr(new Leaf(posElement, makeTree(depth+1, leftTuple), makeTree(depth+1, rightTuple)));
};

