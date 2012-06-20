#ifndef MESH_H
#define MESH_H

#include <string>
#include <array>

#include "vertex.hpp"
#include "normal.hpp"
#include "TokenizedFile.hpp"

class OffLoader
{
    public:
        OffLoader();
        virtual ~OffLoader();
        virtual VertexList readOff(const std::string& fileName,float scale);

    protected:
        bool parseFirstLine(Tokens tokens);
        bool parseSecondLine(Tokens tokens);
        const VertexPtr parseVertex(const Tokens& tokens,float scale) const;

    private:
        unsigned int m_NumVertices;
        unsigned int m_NumNormals;
};

#endif // MESH_H

