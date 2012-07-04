#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <memory>
#include "vec.hpp"
#include "normal.hpp"

using namespace std;

class Vertex;
typedef shared_ptr< Vertex > VertexPtr;
typedef vector< VertexPtr > VertexList;
typedef shared_ptr< VertexList > VertexListPtr;
typedef vector< VertexListPtr > VertexListPtrVector;

class Vertex: public vec3f
{
    public:
        /** Default constructor */
        Vertex(const vec3f pos);
        Vertex(const vec3f pos, NormalPtr normal);
        Vertex(const float x, const float y, const float z);
        Vertex(const float x, const float y, const float z, NormalPtr normal);
        Vertex(const float x, const float y, const float z, const vec3f color);
        Vertex(const vec3f pos, float w);
        Vertex(const float x, const float y, const float z, float w);
        Vertex(const float x, const float y, const float z, const vec3f color, float w);
        virtual ~Vertex();

        void draw(bool useAlpha) const;
		void draw() const {draw(false);}
        void highlight(const vec3f color = vec3f(0.6, 0.5, 0.4));
        void resetColor();
        const float& getFunValue() const { return m_W; };
        void setFunValue(float val) { m_W = val; };
        NormalPtr getNormal() const { return m_NormalPtr; }
		void setNormal(NormalPtr normal){ m_NormalPtr = normal; };

    protected:
        vec3f m_Color;

        NormalPtr m_NormalPtr;
        //Function Value
        float m_W;
    private:
        Vertex() {};
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

