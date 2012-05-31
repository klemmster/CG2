#ifndef GRID_HPP_FY7TIIFJ
#define GRID_HPP_FY7TIIFJ

#include "tree.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>

class Grid
{
public:
    Grid () {};
    Grid (KDTree tree, const size_t dim_x, const size_t dim_y);
    virtual ~Grid () {};

    void approximateLS(VertexList& resultList);
    void approximateWLS(VertexList& resultList);
    void approximateTensor(const size_t k);
    void repeatedApproximation(const size_t k);
    void draw();
    void enableQuads();
    void disableQuads();
    void setK(const size_t k);
    void setH(const float);
    void setRadius(const float radius);
    void reapproximateWLS();
    size_t m_k;

private:

    VertexPtr getByDeCasteljau(const float weight, const size_t iteration,
            const size_t pointNum, const VertexList srcList, size_t count, VertexList& tangentsList);
    void drawTriangles();
    void drawQuads();
    void resetBaseVertices();


    float getWendland(const float distance) const;
    float m_MinX;
    float m_MinY;
    float m_MinZ;
    float m_MaxX;
    float m_MaxY;
    float m_MaxZ;

    VertexList m_vertices;
    VertexList m_interpolVertices;
    std::vector<Eigen::VectorXf> m_coefficients;
    std::vector<vec3f> m_quadNormals;

    KDTree m_tree;
    size_t m_dimX;
    size_t m_dimY;

    float m_h;
    float m_radius;
    bool m_showQuads;
    bool m_showTriangles;

};

#endif /* end of include guard: GRID_HPP_FY7TIIFJ */

