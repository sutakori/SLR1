#ifndef LEXER_TOKEN
#define LEXER_TOKEN

#include <string>

#include "reader.h"
#include "tag.h"
using std::string;

class Token{
public:
  Tag tag;
  string lexeme;

  Token(Tag t){ tag = t; }
  Tag getTag(){return tag;}
  virtual ~Token(){};
  virtual string inspect() = 0;
};

class Tokener{
public:
  Tokener(Reader* reader);
  virtual ~Tokener();
  virtual bool valid_head(char c) = 0;
  virtual Token* forward() = 0;
protected:
  Reader* reader;
};
#endif
