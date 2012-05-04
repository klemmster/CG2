#include "tree.hpp"

#include <algorithm>
#include <float.h>

KDTree::KDTree(const VertexList vertices){

    // Copy Input arrays for individual sorting
    VertexList sortedX(vertices);
    VertexList sortedY(vertices);
    VertexList sortedZ(vertices);

    std::sort(sortedX.begin(), sortedX.end(), Vertex::sortX );
    std::sort(sortedY.begin(), sortedY.end(), Vertex::sortY );
    std::sort(sortedZ.begin(), sortedZ.end(), Vertex::sortZ );
    ListTriple t = { {sortedX, sortedY, sortedZ} };
    Boundaries boundaries = {{ FLT_MIN, FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN, FLT_MAX }};
    m_root = makeTree(0, t, boundaries);


    //TODO: DELETE
        findInRadius(vertices.at(0), 4);
    //END TODO DELETE
};

KDTree::~KDTree(){

};

NodePtr KDTree::makeTree(size_t depth, ListTriple t, Boundaries boundaries){
    /*
     * Tuple contains x, y, z  Dimensions Vertex list
     *
    */
    const size_t k = depth % 3;
    VertexList vertices = t.at(k);

    if(vertices.size() == 0){
        return nullptr;
    }
    if(vertices.size() == 1){
        //IF there is just one point left, the boundaries are the point itself
        VertexPtr vertex = vertices.at(0);
        return NodePtr(new Node(vertices.at(0), nullptr, nullptr, boundaries));
    }

    size_t median = (int) (vertices.size()-1)/2;
    VertexPtr posElement = vertices.at(median);

    ListPair xPair = splitListBy(k, std::get<0>(t), posElement);
    ListPair yPair = splitListBy(k, std::get<1>(t), posElement);
    ListPair zPair = splitListBy(k, std::get<2>(t), posElement);

    ListTriple left = { {std::get<0>(xPair), std::get<0>(yPair), std::get<0>(zPair)} };
    ListTriple right = { {std::get<1>(xPair), std::get<1>(yPair), std::get<1>(zPair)} };

    Boundaries leftBounds = boundaries;
    Boundaries rightBounds = boundaries;
    leftBounds.at(k*2+1) = (*posElement)[k];
    rightBounds.at(k*2) = (*posElement)[k];

    return NodePtr(new Node(posElement, makeTree(depth+1, left, leftBounds),
            makeTree(depth+1, right, rightBounds), boundaries));
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

void KDTree::findInRadius(const NodePtr& src, const HyperSphere& sphere,
            VertexList& result) const{

    if(sphere.inRegion(src->getPosition())){
        result.push_back(src->getPosition());
    }
    if(src->getLeft() != nullptr){
        if(sphere.intersectsRegion(src->getBoundaries())){
            findInRadius(src->getLeft(), sphere, result);
        }
    }
    if(src->getRight() != nullptr){
        if(sphere.intersectsRegion(src->getBoundaries())){
            findInRadius(src->getRight(), sphere, result);
        }
    }
}

VertexList KDTree::findInRadius(const VertexPtr source, const size_t radius){
    VertexList result;
    HyperSphere hyperSphere(source, radius);

    findInRadius(m_root, hyperSphere, result);
    for(VertexPtr ptr : result){
        ptr->highlight();
    }
    return result;
}
