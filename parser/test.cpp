#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
#include "../lexer/lexer.h"
#include "tokens.h"
#include "../lexer/token.h"
using namespace std;

int main(){
  vector<string> files;
  system("ls -l test |grep .txt|awk '{print $9}' > test/ls");
  ifstream fin;
  fin.open("./test/ls", ios::in);
  string name;
  Lexer* lexer;
  Parser* parser = new Parser();
  while(getline(fin,name)){
    lexer = new Lexer("./test/"+name);
    lexer->scan();
    auto tokens = lexer->getTokens();
    auto ret = parser->parse(tokens);
    if(ret)
      parser->view_tree(ret);
  }
  return 0;
}
