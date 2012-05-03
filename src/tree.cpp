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
    ListTriple t = { sortedX, sortedY, sortedZ };
    m_root = makeTree(0, t);
};

KDTree::~KDTree(){

};

NodePtr KDTree::makeTree(size_t depth, ListTriple t){
    /*
     * Tuple contains x, y, z  Dimensions Vertex list
     *
    */
    const size_t axis = depth % 3;
    VertexList vertices = t.at(axis);

    if(vertices.size() == 0){
        return nullptr;
    }
    if(vertices.size() == 1){
        return NodePtr(new Node(vertices.at(0), nullptr, nullptr));
    }

    size_t median = (int) (vertices.size()-1)/2;
    VertexPtr posElement = vertices.at(median);

    ListPair xPair = splitListBy(0, std::get<0>(t), posElement);
    ListPair yPair = splitListBy(1, std::get<1>(t), posElement);
    ListPair zPair = splitListBy(2, std::get<2>(t), posElement);

    ListTriple left = {std::get<0>(xPair), std::get<0>(yPair), std::get<0>(zPair)};
    ListTriple right = {std::get<1>(xPair), std::get<1>(yPair), std::get<1>(zPair)};

    return NodePtr(new Node(posElement, makeTree(depth+1, left),
            makeTree(depth+1, right)));
};


ListPair KDTree::splitListBy(const size_t index, const VertexList sourceList,
        const VertexPtr sourceVert){
    VertexList left;
    VertexList right;

    for(auto elem : sourceList){
        if(elem != sourceVert){
            if((*elem)[index] < (*sourceVert)[index]){
                left.push_back(elem);
            }else{
                right.push_back(elem);
            }
        }
    }
    return ListPair(left, right);
}

VertexList KDTree::findNearestNeighbour(const VertexPtr source){
    VertexList result;

    return result;
}

VertexList KDTree::findInRadius(const VertexPtr source, const size_t radius){
    VertexList result;

    return result;
}
