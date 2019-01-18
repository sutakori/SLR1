#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "lexer.h"
using std::vector;
using std::string;
using namespace std;

int main(){
  vector<string> files;
  system("ls -l test |grep .c|awk '{print $9}' > test/ls.txt");
  ifstream fin;
  fin.open("./test/ls.txt", ios::in);
  string name;
  Lexer* lexer;
  while(getline(fin,name)){
    lexer = new Lexer("./test/"+name);
    lexer->scan();
    lexer->print();
    cout<<endl;
  }
  return 0;
}
