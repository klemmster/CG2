#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <memory>
#include "vec.hpp"

#ifdef __APPLE__
#include <boost/smart_ptr.hpp>

using namespace boost;
#endif

using namespace std;

class Vertex
{
    public:
        /** Default constructor */
        Vertex(const vec3f pos);
        Vertex(const float x, const float y, const float z);

        virtual ~Vertex();
    
        vec3f m_pos;

    protected:
    private:
        
        Vertex() {};
};
typedef shared_ptr< Vertex > VertexPtr;
typedef vector< VertexPtr > VertexList;


#endif // VERTEX_H
