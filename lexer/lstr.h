#ifndef LEXER_LSTR
#define LEXER_LSTR

#include "token.h"
#include "tag.h"

class Lstrer: public Tokener{
public:
  Lstrer(Reader* reader);
  bool valid_head(char c);
  Token* forward();
};

class Lstr: public Token{
public:
  Lstr();
  ~Lstr();
  string inspect();
};


#endif
