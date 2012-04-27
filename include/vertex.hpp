#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <memory>
#include "vec.hpp"

using namespace std;

class Vertex
{
    public:
        /** Default constructor */
        Vertex(const vec3f pos);
        Vertex(const float x, const float y, const float z);

        virtual ~Vertex();

    protected:
    private:
        vec3f m_pos;
        Vertex() {};
};
typedef shared_ptr< Vertex > VertexPtr;
typedef vector< VertexPtr > VertexList;


#endif // VERTEX_H
