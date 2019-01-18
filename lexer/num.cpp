#include "token.h"
#include "tag.h"
#include "num.h"

Int::Int(Tag tag, int v):Token(tag){
  this->value = v;
}
Int::~Int(){}
string Int::inspect(){
  char dest[20];
  sprintf(dest, "<%s, %d>", get_tag_name(tag).c_str(), value);
  string ret = dest;
  return ret;
}

Real::Real(Tag tag, float v):Token(tag){
  this->value = v;
}
Real::~Real(){}

string Real::inspect(){
  char dest[20];
  sprintf(dest, "<%s, %f>", get_tag_name(tag).c_str(), value);
  string ret = dest;
  return ret;
}

Numer::Numer(Reader* reader):Tokener(reader){}

bool Numer::valid_head(char c){
  return isdigit(c);
}

Token* Numer::forward(){
  int v = 0;
  Token* ret;
  char head;
  while(isdigit(head = reader->peek())){
    v = 10*v + (head-'0');
    reader->forward();
  }
  if(head != '.'){
    ret = new Int(Tag::INTEGER, v);
  } else{
    reader->forward();
    float x = v;
    float d = 10; //weight of digit
    while(isdigit(head = reader->peek())){
      x = x + (float)(head-'0') / d;
      d *= 10;
      reader->forward();
    }
    ret = new Real(Tag::REAL, x);
  }
  ret->lexeme = reader->confirm();
  return ret;
}
