#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#if defined(WIN32)
#define M_PI 3.14159265358979323846
#endif

class Object
{
    typedef std::shared_ptr< Object > ObjectPtr;

public:
    Object( const VertexList vertices);
    virtual ~Object() {}

    virtual void draw() = 0;

protected:

private:

    VertexList m_Vertices;
};
#endif

