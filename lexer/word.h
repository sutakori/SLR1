#ifndef LEXER_WORD
#define LEXER_WORD

#include <string>
#include <map>

#include "token.h"
#include "tag.h"
using std::string;
using std::map;

class SF:public Token{
public:
  SF(Tag tag);
  string inspect();
};

class ID:public Token{
public:
  string name;
  ID(string name);
  string inspect();
};

class Worder:public Tokener{
public:
  Worder(Reader* reader);
  bool valid_head(char c);
  Token* forward();
private:
  inline bool valid_tail(char c);
  void put_reserve(string lexeme, Tag tag);
  map<string, Tag> reserve_words;
};

#endif
