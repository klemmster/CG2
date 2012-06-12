#ifndef MESH_H
#define MESH_H

#include <string>

#include "vertex.hpp"
#include "normal.hpp"
#include "TokenizedFile.hpp"

class OffLoader
{
    public:
        OffLoader();
        virtual ~OffLoader();
        virtual VertexList readOff(const std::string& fileName);

    protected:
        bool parseFirstLine(Tokens tokens);
        bool parseSecondLine(Tokens tokens);
        const VertexPtr parseVertex(const Tokens& tokens) const;
        const NormalPtr parseNormal(const Tokens& tokens) const;
    private:
        unsigned int m_NumVertices;
        unsigned int m_NumNormals;
};

#endif // MESH_H

