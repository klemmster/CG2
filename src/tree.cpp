#include "tree.hpp"

#include <algorithm>
#include <vector>

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>

using namespace boost;

#else
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;
typedef shared_ptr< thread > threadPtr;

#endif

#include <float.h>
#include "stopwatch.hpp"
KDTree::KDTree(const VertexList vertices, const size_t dimensions):
    m_K(dimensions)
{

    VertexLists lists;
    lists.reserve(m_K);
    // Copy Input arrays for individual sorting
    Stopwatch taS("SortArrays");

    //Create Lists
    for(size_t i = 0; i<m_K; ++i){
        lists.push_back(VertexList(vertices));
    }

    for(size_t i = 0; i<m_K; ++i){
        SortVertex sortByDim(i);
       sort(lists.at(i).begin(), lists.at(i).end(), sortByDim );
    }

    /*
    vector< threadPtr > sortThreads;
    //Sort Lists threaded
    for(size_t i = 0; i<m_K; ++i){
        SortVertex sortByDim(i);
        threadPtr sortListTh(new thread([&] { sort(lists.at(i).begin(), lists.at(i).end(), sortByDim ); }));
        sortThreads.push_back(sortListTh);
    }
    */

    /*
    //Wait for sorting to complete
    for(threadPtr th : sortThreads){
        th->join();
    }
    */

    //Extract minimum and maximum values
    for(VertexList list : lists){
        m_MinVertices.push_back(list.front());
        m_MaxVertices.push_back(list.back());
    }

    taS.stop();

    vector<float> domainMins;
    vector<float> domainMaxs;
    domainMins.push_back((*m_MinVertices.at(0))[0]);
    domainMins.push_back((*m_MinVertices.at(1))[1]);
    domainMins.push_back((*m_MinVertices.at(2))[2]);
    domainMaxs.push_back((*m_MaxVertices.at(0))[0]);
    domainMaxs.push_back((*m_MaxVertices.at(1))[1]);
    domainMaxs.push_back((*m_MaxVertices.at(2))[2]);

    Domain domain( domainMins, domainMaxs);
    Stopwatch mkTreeS("maketree");
    m_root = makeTree(0, 251, lists, domain);
    mkTreeS.stop();
};

KDTree::KDTree():
    m_K(3)
{

}

KDTree::~KDTree(){

};

NodePtr KDTree::makeTree(size_t depth, const size_t& cellSize, VertexLists& t,
        const Domain& domain){
    /*
     * Tuple contains x, y, z  Dimensions Vertex list
     *
    */
    const size_t k = depth % m_K;
    VertexList vertices = t.at(k);

    if(vertices.size() == 0){
        return nullptr;
    }
    if(vertices.size() <= cellSize){
        return NodePtr(new Node(vertices, domain));
    }

    size_t median = (int) (vertices.size()-1)/2;
    VertexPtr& posElement = vertices.at(median);

    //Split lists by median element
    std::vector< ListPair > pairs;
    for(size_t i=0; i<m_K; ++i){
        pairs.push_back(splitListBy(k, t.at(i), posElement));
    }

    VertexLists left;
    VertexLists right;
    for(ListPair pair: pairs){
        left.push_back(std::get<0>(pair));
        right.push_back(std::get<1>(pair));
    }

    Domain leftBounds = domain;
    Domain rightBounds = domain;
    leftBounds.updateMax((*posElement)[k], k);
    rightBounds.updateMin((*posElement)[k], k);

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
    return NodePtr(new Node(leftNode, rightNode, domain));
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

        if(results.full() && sphere.withinRegion(src->getDomain())){
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
        if(sphere.intersectsRegion(src->getLeft()->getDomain())){
            findKNearestNeighbours(src->getLeft(), results, target);
        }
    }
    if(src->getRight()){
        if(sphere.intersectsRegion(src->getRight()->getDomain())){
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
        if(sphere.intersectsRegion(src->getDomain())){
            findInRadius(src->getLeft(), sphere, result);
        }
    }
    if(src->getRight() != nullptr){
        if(sphere.intersectsRegion(src->getDomain())){
            findInRadius(src->getRight(), sphere, result);
        }
    }
}

VertexList KDTree::findInRadius(const VertexPtr source, const double radius){
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

    Domain domain = src->getDomain();
    domain.draw();

    glBegin(GL_LINES);

    /*
    glVertex3f(domain.getMin(0], bounds[2], bounds[4]);
    glVertex3f(domain.getMin(0], bounds[2], bounds[5]);

    glVertex3f(domain.getMin(0], bounds[2], bounds[5]);
    glVertex3f(domain.getMin(0], bounds[3], bounds[5]);

    glVertex3f(domain.getMin(0], bounds[3], bounds[5]);
    glVertex3f(domain.getMin(0], bounds[3], bounds[4]);

    glVertex3f(domain.getMin(0], bounds[3], bounds[4]);
    glVertex3f(domain.getMin(0], bounds[2], bounds[4]);

    glVertex3f(domain.getMin(0], bounds[2], bounds[4]);
    glVertex3f(domain[1], bounds[2], bounds[4]);

    glVertex3f(domain.getMin(0], bounds[3], bounds[4]);
    glVertex3f(domain[1], bounds[3], bounds[4]);

    glVertex3f(domain[1], bounds[3], bounds[4]);
    glVertex3f(domain[1], bounds[2], bounds[4]);

    glVertex3f(domain[1], bounds[3], bounds[4]);
    glVertex3f(domain[1], bounds[3], bounds[5]);

    glVertex3f(domain[1], bounds[2], bounds[4]);
    glVertex3f(domain[1], bounds[2], bounds[5]);

    glVertex3f(domain[1], bounds[3], bounds[5]);
    glVertex3f(domain[1], bounds[2], bounds[5]);

    glVertex3f(domain[1], bounds[2], bounds[5]);
    glVertex3f(domain.getMin(0], bounds[2], bounds[5]);

    glVertex3f(domain[1], bounds[3], bounds[5]);
    glVertex3f(domain.getMin(0], bounds[3], bounds[5]);

    */
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

