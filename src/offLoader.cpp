#include "offLoader.hpp"

#include <iostream>
#include <boost/format.hpp>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>

OffLoader::OffLoader():
     m_NumVertices(0)
    {}

OffLoader::~OffLoader()
{
     //dtor
}

VertexList OffLoader::readOff(const std::string& fileName)
{
    TokenizedFile file(fileName);
    VertexList vertices;
    NormalList normals;
    if(parseFirstLine(file.next())) {
       if(parseSecondLine(file.next())) {
            for(unsigned int i=0; i<m_NumVertices; i++) {
                vertices.push_back(parseVertex(file.next()));
            }
       }
    }
    return vertices;
}

TupleVerticesNormals OffLoader::readNOff(const std::string& fileName)
{
    TokenizedFile file(fileName);
    VertexList vertices;
    NormalList normals;

    if(parseFirstLine(file.next())) {
        if(parseSecondLine(file.next())) {
            for(unsigned int i=0; i<m_NumVertices; i++) {

                tuple<VertexPtr, NormalPtr> tuVerNorm = parseVertexAndNormal(file.next());

                vertices.push_back(get<0>(tuVerNorm));
                normals.push_back(get<1>(tuVerNorm));
            }
        }
    }

    return TupleVerticesNormals(vertices, normals);
}

bool OffLoader::parseFirstLine(Tokens tokens)
{
     //First and Only Token should equal OFF, and there shoul be just one token
     assert(tokens.size() == 1);
     std::string token = tokens.at(0);
     if(!(boost::iequals(token, "off") || boost::iequals(token, "noff"))) {
          std::cout << token << std::endl;
          std::cerr << "First Line expected: 'OFF'" << std::endl;
          return false;
     }
     return true;
}

bool OffLoader::parseSecondLine(Tokens tokens)
{
     //There should be three tokens here
     assert(tokens.size() == 3);
     m_NumVertices = std::atoi(tokens.at(0).c_str());
     m_NumNormals = std::atoi(tokens.at(0).c_str());
     std::cout << boost::format("%s Vertices") % m_NumVertices << std::endl;
     return true;
}

const VertexPtr OffLoader::parseVertex(const Tokens& tokens) const
{
     assert(tokens.size() == 3);
     /*
     float x = std::atof(tokens.at(0).c_str());
     float y = std::atof(tokens.at(1).c_str());
     float z = std::atof(tokens.at(2).c_str());
     */
     float x = boost::lexical_cast<float>(tokens.at(0));
     float y = boost::lexical_cast<float>(tokens.at(1));
     float z = boost::lexical_cast<float>(tokens.at(2));
     return VertexPtr(new Vertex(x, y, z));
}

const tuple<VertexPtr, NormalPtr> OffLoader::parseVertexAndNormal(const Tokens& tokens) const
{
    assert(tokens.size() == 6);

    float x = boost::lexical_cast<float>(tokens.at(0));
    float y = boost::lexical_cast<float>(tokens.at(1));
    float z = boost::lexical_cast<float>(tokens.at(2));

    float dirX = boost::lexical_cast<float>(tokens.at(3));
    float dirY = boost::lexical_cast<float>(tokens.at(4));
    float dirZ = boost::lexical_cast<float>(tokens.at(5));
    return tuple<VertexPtr, NormalPtr>(VertexPtr(new Vertex(x, y, z)), NormalPtr(new Normal( normalize( vec3f(dirX, dirY, dirZ) ) )));
}

