#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <memory>
#include "vec.hpp"

using namespace std;

class Vertex;
typedef shared_ptr< Vertex > VertexPtr;
typedef vector< VertexPtr > VertexList;

class Vertex: public vec3f
{
    public:
        /** Default constructor */
        Vertex(const vec3f pos);
        Vertex(const float x, const float y, const float z);
        Vertex(const float x, const float y, const float z, const vec3f color);
        virtual ~Vertex();

        void draw() const;
        void highlight(const vec3f color = vec3f(1.0, 0.0, 0.0));
        void resetColor();

    protected:
    private:

        Vertex() {};
        vec3f m_Color;
};

class SortVertex
{
public:
    SortVertex (const size_t dim):m_dim(dim){};
    virtual ~SortVertex (){};

    bool operator() (const VertexPtr a, const VertexPtr b){
            return ((*a)[m_dim] < (*b)[m_dim]);
            };
private:
    const size_t m_dim;
};

#endif // VERTEX_H

