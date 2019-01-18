#ifndef LEXER_ERROR
#define LEXER_ERROR

#include <string>

#include "tag.h"
#include "token.h"
using std::string;

enum class ErrTag
  {
   STRAY_CHAR,
   UNTERMINATED_COMMENT,
   UNTERMINATED_STRING
  };

class Err: public Token{
public:
  Tag tag;
  Err(ErrTag err, ReadInfo info);
  ~Err();
  string inspect();
private:
  string errIndi();
  ErrTag errtag;
  ReadInfo info;
};

#endif
