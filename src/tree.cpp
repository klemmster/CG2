#include "tree.hpp"

#include <algorithm>

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>

using namespace boost;

#else
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

#endif

#include <float.h>
#include "stopwatch.hpp"
KDTree::KDTree(const VertexList vertices){


    // Copy Input arrays for individual sorting
    Stopwatch taS("SortArrays");

    VertexList sortedX(vertices);
    VertexList sortedY(vertices);
    VertexList sortedZ(vertices);

    thread xSorted([&] { std::sort(sortedX.begin(), sortedX.end(), Vertex::sortX); });
    thread ySorted([&] { std::sort(sortedY.begin(), sortedY.end(), Vertex::sortY); });
    thread zSorted([&] { std::sort(sortedZ.begin(), sortedZ.end(), Vertex::sortZ); });
    xSorted.join();
    ySorted.join();
    zSorted.join();
    m_MinVertices.push_back(sortedX.front());
    m_MinVertices.push_back(sortedY.front());
    m_MinVertices.push_back(sortedZ.front());
    m_MaxVertices.push_back(sortedX.back());
    m_MaxVertices.push_back(sortedY.back());
    m_MaxVertices.push_back(sortedZ.back());
    taS.stop();


    float xMin = (*sortedX.front())[0];
    float xMax = (*sortedX.back())[0];
    float yMin = (*sortedY.front())[1];
    float yMax = (*sortedY.back())[1];
    float zMin = (*sortedZ.front())[2];
    float zMax = (*sortedZ.back())[2];
    ListTriple t = { {sortedX, sortedY, sortedZ} };
    Boundaries boundaries = {{ xMin, xMax, yMin, yMax, zMin, zMax }};
    Stopwatch mkTreeS("maketree");
    m_root = makeTree(0, 251, t, boundaries);
    mkTreeS.stop();


};

KDTree::KDTree(){

}

KDTree::~KDTree(){

};

NodePtr KDTree::makeTree(size_t depth, const size_t& cellSize, ListTriple& t,
        const Boundaries& boundaries){
    /*
     * Tuple contains x, y, z  Dimensions Vertex list
     *
    */
    const size_t k = depth % 3;
    VertexList vertices = t.at(k);

    if(vertices.size() == 0){
        return nullptr;
    }
    if(vertices.size() <= cellSize){
        return NodePtr(new Node(vertices, boundaries));
    }

    size_t median = (int) (vertices.size()-1)/2;
    VertexPtr& posElement = vertices.at(median);

    ListPair xPair = splitListBy(k, std::get<0>(t), posElement);
    ListPair yPair = splitListBy(k, std::get<1>(t), posElement);
    ListPair zPair = splitListBy(k, std::get<2>(t), posElement);

    ListTriple left = { {std::get<0>(xPair), std::get<0>(yPair), std::get<0>(zPair)} };
    ListTriple right = { {std::get<1>(xPair), std::get<1>(yPair), std::get<1>(zPair)} };

    Boundaries leftBounds = boundaries;
    Boundaries rightBounds = boundaries;
    leftBounds.at(k*2+1) = (*posElement)[k];
    rightBounds.at(k*2) = (*posElement)[k];

    NodePtr leftNode;
    NodePtr rightNode;
    if(depth < 2){
    thread lT([&] { leftNode = makeTree(depth+1, cellSize, left, leftBounds); });
    thread rT([&] { rightNode = makeTree(depth+1, cellSize, right, rightBounds); });
    lT.join();
    rT.join();
    }else{
        leftNode = makeTree(depth+1, cellSize, left, leftBounds);
        rightNode = makeTree(depth+1, cellSize, right, rightBounds);
    }
    return NodePtr(new Node(leftNode, rightNode, boundaries));
};


ListPair KDTree::splitListBy(const size_t& index, const VertexList& sourceList,
        const VertexPtr& sourceVert){
    VertexList left;
    VertexList right;

    for(VertexPtr elem : sourceList){
        if((*elem)[index] < (*sourceVert)[index]){
            left.push_back(elem);
        }else{
            right.push_back(elem);
        }
    }
    return ListPair(left, right);
}

VertexList KDTree::findKNearestNeighbours(const VertexPtr source,
        const size_t numNeighbours){
    VertexList result;
    Stopwatch findS("NKSearch");
    LimitedPriorityQueue resultQueue(numNeighbours);
    findKNearestNeighbours(m_root, resultQueue, source);
    while(!resultQueue.empty()){
        VertexDistPair pair = resultQueue.top();
        VertexPtr vrtx = std::get<0>(pair);
        result.push_back(vrtx);
        vrtx->highlight();
        resultQueue.pop();
    }
    findS.stop();
    return result;
}

void KDTree::findKNearestNeighbours(const NodePtr& src, LimitedPriorityQueue& results,
        const VertexPtr& target){

    if(src->isLeaf()){
        for(VertexPtr vrtx: src->getBucket()){
            float dist = norm((*vrtx)-(*target));
            results.push(VertexDistPair(vrtx, dist));
        }
        HyperSphere sphere(target, std::get<1>(results.top()));
        //Required Number of points are found, sphere was completely in region,
        //there can't be any closer results

        if(results.full() && sphere.withinRegion(src->getBoundaries())){
            return;
        }
    }
    //Uncool, get sphere again
    float dist = FLT_MAX;
    if(!results.empty()){
        dist = std::get<1>(results.top());
    }
    HyperSphere sphere(target, dist );

    if(src->getLeft()){
        if(sphere.intersectsRegion(src->getLeft()->getBoundaries())){
            findKNearestNeighbours(src->getLeft(), results, target);
        }
    }
    if(src->getRight()){
        if(sphere.intersectsRegion(src->getRight()->getBoundaries())){
            findKNearestNeighbours(src->getRight(), results, target);
        }
    };
};

void KDTree::findInRadius(const NodePtr& src, const HyperSphere& sphere,
            VertexList& result) const{

    if(src->isLeaf()){
        for(VertexPtr vrtx : src->getBucket()){
            if(sphere.contains(vrtx)){
                result.push_back(vrtx);
            }
        }
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
    Stopwatch findS("Radius - Search");
    HyperSphere hyperSphere(source, radius);
    findInRadius(m_root, hyperSphere, result);
    findS.stop();
    for(VertexPtr ptr : result){
        ptr->highlight();
    }
    return result;
}

void KDTree::draw() {
    // std::cout << "Drawing KDTree" << std::endl;

    glColor3f(0, 1, 0);
    drawSingleNode(m_root);

}

void KDTree::drawSingleNode(const NodePtr &src) {

    if (src == nullptr) {
        return;
    }

    std::array<float, 6> bounds = src->getBoundaries();

    glBegin(GL_LINES);

    glVertex3f(bounds[0], bounds[2], bounds[4]);
    glVertex3f(bounds[0], bounds[2], bounds[5]);

    glVertex3f(bounds[0], bounds[2], bounds[5]);
    glVertex3f(bounds[0], bounds[3], bounds[5]);

    glVertex3f(bounds[0], bounds[3], bounds[5]);
    glVertex3f(bounds[0], bounds[3], bounds[4]);

    glVertex3f(bounds[0], bounds[3], bounds[4]);
    glVertex3f(bounds[0], bounds[2], bounds[4]);

    glVertex3f(bounds[0], bounds[2], bounds[4]);
    glVertex3f(bounds[1], bounds[2], bounds[4]);

    glVertex3f(bounds[0], bounds[3], bounds[4]);
    glVertex3f(bounds[1], bounds[3], bounds[4]);

    glVertex3f(bounds[1], bounds[3], bounds[4]);
    glVertex3f(bounds[1], bounds[2], bounds[4]);

    glVertex3f(bounds[1], bounds[3], bounds[4]);
    glVertex3f(bounds[1], bounds[3], bounds[5]);

    glVertex3f(bounds[1], bounds[2], bounds[4]);
    glVertex3f(bounds[1], bounds[2], bounds[5]);

    glVertex3f(bounds[1], bounds[3], bounds[5]);
    glVertex3f(bounds[1], bounds[2], bounds[5]);

    glVertex3f(bounds[1], bounds[2], bounds[5]);
    glVertex3f(bounds[0], bounds[2], bounds[5]);

    glVertex3f(bounds[1], bounds[3], bounds[5]);
    glVertex3f(bounds[0], bounds[3], bounds[5]);

    glEnd();

    drawSingleNode(src->getLeft());
    drawSingleNode(src->getRight());
}

void KDTree::draw(const VertexList vertices) {

}

const VertexList& KDTree::getMinVertices() const{
    return m_MinVertices;
}

const VertexList& KDTree::getMaxVertices() const{
    return m_MaxVertices;
}

