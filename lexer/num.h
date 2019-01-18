#ifndef LEXER_NUM
#define LEXER_NUM

#include "token.h"
#include "tag.h"

class Numer: public Tokener{
public:
  Numer(Reader* reader);
  bool valid_head(char c);
  Token* forward();
};

class Int: public Token{
public:
  int value;
  Int(Tag tag, int v);
  ~Int();
  string inspect();
};

class Real: public Token{
public:
  float value;
  Real(Tag tag, float v);
  ~Real();
  string inspect();
};
#endif
