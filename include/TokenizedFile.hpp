#ifndef TOKENIZEDFILE_HPP_XSDLZF62
#define TOKENIZEDFILE_HPP_XSDLZF62

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <exception>

typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
typedef std::vector< std::string > Tokens;

class FileDoesNotExistException: public std::exception
{
  virtual const char* what() const throw() //override
  {
    return "File does not exist";
  }
};


class TokenizedFile
{

public:
    TokenizedFile (const std::string& fileName);
    Tokens next();
    virtual ~TokenizedFile ();

private:
    boost::filesystem::fstream m_File;
    boost::char_separator<char> m_sep;
    std::string m_Line;
};

#endif /* end of include guard: TOKENIZEDFILE_HPP_XSDLZF62 */

