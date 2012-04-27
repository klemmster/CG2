#include "TokenizedFile.hpp"

using namespace boost::filesystem3;

TokenizedFile::TokenizedFile(const std::string& fileName){
    //  "/=" is the append path operator
    path filePath = current_path() /=path("meshes") /= path(fileName);
    if(exists(filePath)) {
        m_File.open(filePath);
    }else{
        throw FileDoesNotExistException();
    }
}

TokenizedFile::~TokenizedFile(){
    m_File.close();
}

Tokens TokenizedFile::next(){
     Tokens tokens;
     std::string line;
     boost::char_separator<char> sep(" ", "\n");
     getline(m_File, line);
     //Get rid of leading and trailing zeros
     boost::trim(line);
     Tokenizer tok(line, sep);
     tokens.assign(tok.begin(), tok.end());
     return tokens;
}
