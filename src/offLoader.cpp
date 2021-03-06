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

VertexList OffLoader::readOff(const std::string& fileName,float scale)
{
    TokenizedFile file(fileName);
    VertexList vertices;

    if(parseFirstLine(file.next())) {
        if(parseSecondLine(file.next())) {
            for(unsigned int i=0; i<m_NumVertices; i++) {

                VertexPtr vertex = parseVertex(file.next(),scale);
                vertices.push_back(vertex);
            }
        }
    }

    std::cout << "NumVertices: " <<  vertices.size() << "\n";
    return vertices;
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

const VertexPtr OffLoader::parseVertex(const Tokens& tokens,float scale) const
{
    assert(tokens.size() == 6);

    float x = boost::lexical_cast<float>(tokens.at(0)) * scale;
    float y = boost::lexical_cast<float>(tokens.at(1)) * scale;
    float z = boost::lexical_cast<float>(tokens.at(2)) * scale;

    float dirX = boost::lexical_cast<float>(tokens.at(3));
    float dirY = boost::lexical_cast<float>(tokens.at(4));
    float dirZ = boost::lexical_cast<float>(tokens.at(5));

    NormalPtr normal = NormalPtr(new Normal( normalize( vec3f(dirX, dirY, dirZ) )));
    VertexPtr vertex = VertexPtr(new Vertex(x, y, z, normal));
    return vertex;
}

