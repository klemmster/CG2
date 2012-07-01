#ifndef MARCHINGCUBE_HPP_4GH0L5D6
#define MARCHINGCUBE_HPP_4GH0L5D6

#include "Grid3D.hpp"
#include <vector>

typedef char int8;
typedef std::pair<VertexPtr, VertexPtr> Edge;
class Cube
{
public:
    //Cube Numbering as described in http://local.wasp.uwa.edu.au/~pbourke/geometry/polygonise/
    Cube(VertexPtr vrtx0, VertexPtr vrtx1, VertexPtr vrtx2, VertexPtr vrtx3,
            VertexPtr vrtx4, VertexPtr vrtx5, VertexPtr vrtx6, VertexPtr vrtx7);
    virtual ~Cube();

    VertexList m_vertices;
    std::vector<Edge> m_Edges;

    int getIndex();
    VertexList getTriangles();
    VertexPtr getInterpolatedVertex(const Edge& edge);
};

class MarchingCubes
{
public:
    MarchingCubes();
    MarchingCubes (const Grid3D& grid, const size_t dimX, const size_t dimY,
            const size_t dimZ);
    virtual ~MarchingCubes ();

    void march();
    void draw();
    VertexList getVertices() { return m_triangles; }

protected:

private:
    Grid3D m_Grid;
    size_t m_dimX;
    size_t m_dimY;
    size_t m_dimZ;

    VertexList m_triangles;
};


#endif /* end of include guard: MARCHINGCUBE_HPP_4GH0L5D6 */

