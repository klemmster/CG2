#include "cg2_framework.h"

#include <cmath>

using namespace std;


// ----------------------------------------------------------------
float EuclDist::dist(Point& a, Point& b) {
    // ----------------------------------------------------------------
    float xd = a.x - b.x;
    float yd = a.y - b.y;
    float zd = a.z - b.z;
    return sqrt( xd*xd + yd*yd + zd*zd );
}

// ----------------------------------------------------------------
float EuclDist::dist(Point& a, Node& n) {
    // ----------------------------------------------------------------
    // return 0.0 if point is within node n
    // and the euclidian distance otherwise
    return 0.0f;

    // TODO etc...
}

// ----------------------------------------------------------------
SDS::SDS(PointList* plist, DistFunc* dfunc) {
    // ----------------------------------------------------------------
    _plist = plist;
    _dfunc = dfunc;

    // dummy datastructure (one node with references to all points)
    _rootnode = new Node();
    for (unsigned int i = 0; i < _plist->size(); i++) {
        _rootnode->plist.push_back(&(*_plist)[i]);
    }

    // TODO build true spatial datastructure etc...
}

// ----------------------------------------------------------------
PointPointerList SDS::collectKNearest(Point& p, int knearest) {
    // ----------------------------------------------------------------

    // !NOTE: bogus implementation of k-nearest neighbor search
    PointPointerList pl;
    int i = 0, numelem = static_cast<int> (_rootnode->plist.size());
    while (i < knearest && i < numelem) {
        pl.push_back(_rootnode->plist[i]);
        i++;
    }
    return pl;

    // TODO use spatial data structure for sub-linear search etc...
}

// ----------------------------------------------------------------
PointPointerList SDS::collectInRadius(Point& p, float radius) {
    // ----------------------------------------------------------------

    // dummy brute force implementation
    PointPointerList plist;
    for (int i = 0; i < static_cast<int> (_plist->size()); i++) {
        if (_dfunc->dist(p, (*_plist)[i]) < radius) {
            plist.push_back(&(*_plist)[i]);
        }
    }
    return plist;

    // TODO use spatial data structure for sub-linear search etc...
}

// ----------------------------------------------------------------
int SDS::size() {
    // ----------------------------------------------------------------
    return static_cast<int> (_plist->size());
}

// ----------------------------------------------------------------
void SDS::draw() {
    // ----------------------------------------------------------------

    // TODO use OpenGL to draw the point data
}

// ----------------------------------------------------------------
void SDS::draw(const PointPointerList& plist) {
    // ----------------------------------------------------------------

    // TODO use OpenGL to draw the point data
}

// ----------------------------------------------------------------
PointList* SDS::getPoints() {
    // ----------------------------------------------------------------
    return _plist;
}

// ----------------------------------------------------------------
ostream& operator << (ostream& os, const SDS& sds) {
    // ----------------------------------------------------------------
    int numelem = static_cast<int> (sds._plist->size());
    os << "num points in set: " << numelem << endl;
    for (int i = 0; i < numelem; i++) {
        os << "point " << i << ": " << (*sds._plist)[i].x << " "
                << (*sds._plist)[i].y << " " << (*sds._plist)[i].z
                << endl;
    }
    return os;
}

// ----------------------------------------------------------------
ostream& operator << (ostream& os, const PointPointerList& l) {
    // ----------------------------------------------------------------
    int numelem = static_cast<int> (l.size());
    os << "num points in set: " << numelem << endl;
    for (int i = 0; i < numelem; i++) {
        os << "point " << i << ": " << (*l[i]).x << " "
                << (*l[i]).y << " " << (*l[i]).z << endl;
    }
    return os;
}
