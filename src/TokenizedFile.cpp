#include "TokenizedFile.hpp"

using namespace boost::filesystem3;

TokenizedFile::TokenizedFile(const std::string& fileName):
    m_sep(boost::char_separator<char>(" ", "\n")){
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
     getline(m_File, m_Line);
     //Get rid of leading and trailing zeros
     boost::trim(m_Line);
     Tokenizer tok(m_Line, m_sep);
     tokens.assign(tok.begin(), tok.end());
     return tokens;
}
