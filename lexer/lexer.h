#ifndef LEXER_LEXER
#define LEXER_LEXER

#include <string>
#include <vector>

#include "reader.h"
#include "token.h"
#include "num.h"
#include "operator.h"
#include "word.h"
#include "space.h"
#include "lstr.h"
using std::string;
using std::vector;

class Lexer{
public:
  Lexer(string filepath);
  ~Lexer();
  /* get the next token
   */
  Token* forward();

  /* scan the entire code
   * generate tokens, error messages and summarizes
   * return:
   *   error count
   */
  int scan();
  void print();
  vector<Token*> getTokens();
private:
  Reader* reader;
  // token constructors
  Spacer* spacer;
  Numer* numer;
  OPer* oper;
  Worder* worder;
  Lstrer* lstrer;

  vector<Token*> tokens;
};

#endif
