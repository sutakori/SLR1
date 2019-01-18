#ifndef LEXER_OPERATOR
#define LEXER_OPERATOR

#include "tag.h"
#include "token.h"

class OP: public Token{
public:
  OP(Tag tag);
  ~OP();
  string inspect();
};

class OPer: public Tokener{
public:
  OPer(Reader* reader);
  bool valid_head(char c);
  Token* forward();
private:
  string valid_heads = "+-*/%&^|~!()[]{}?.,;:=<>";
};

#endif
