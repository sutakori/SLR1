#ifndef LEXER_SPACE
#define LEXER_SPACE

#include "token.h"
#include "tag.h"

class Spacer: public Tokener{
public:
  Spacer(Reader* reader);
  bool valid_head(char c);
  Token* forward();
};

class Space: public Token{
public:
  Space();
  ~Space();
  string inspect();
};

class Comment:public Token{
public:
  Comment();
  ~Comment();
  string inspect();
};
#endif
