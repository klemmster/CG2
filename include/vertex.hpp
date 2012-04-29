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

        static bool sortX(const VertexPtr a, const VertexPtr b) { return ((*a)[0] < (*b)[0]); };
        static bool sortY(const VertexPtr a, const VertexPtr b) { return ((*a)[1] < (*b)[1]); };
        static bool sortZ(const VertexPtr a, const VertexPtr b) { return ((*a)[2] < (*b)[2]); };
        virtual ~Vertex();

    protected:
    private:

        Vertex() {};
};


#endif // VERTEX_H
