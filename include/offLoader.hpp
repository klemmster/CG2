#ifndef MESH_H
#define MESH_H

#include <string>
#include <array>

#include "vertex.hpp"
#include "normal.hpp"
#include "TokenizedFile.hpp"

typedef tuple<VertexList, NormalList> TupleVerticesNormals;

class OffLoader
{
    public:
        OffLoader();
        virtual ~OffLoader();
        virtual VertexList readOff(const std::string& fileName);
        virtual TupleVerticesNormals readNOff(const std::string& fileName);
    
    protected:
        bool parseFirstLine(Tokens tokens);
        bool parseSecondLine(Tokens tokens);
        const VertexPtr parseVertex(const Tokens& tokens) const;
        const tuple<VertexPtr, NormalPtr> parseVertexAndNormal(const Tokens& tokens) const;
    private:
        unsigned int m_NumVertices;
        unsigned int m_NumNormals;
};

#endif // MESH_H

