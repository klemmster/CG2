#ifndef MESH_H
#define MESH_H

#include <string>

#include "vertex.hpp"
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
        const VertexPtr parseVertex(Tokens tokens) const;

    private:
        Tokens getTokens(boost::filesystem::fstream &openFile);
        unsigned int m_NumVertices;
        unsigned int m_NumFaces;
        unsigned int m_NumEdges;
};

#endif // MESH_H

