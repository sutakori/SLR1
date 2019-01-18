#include "token.h"
#include "tag.h"
#include "word.h"

SF::SF(Tag tag): Token(tag){}
string SF::inspect(){
  char dest[20];
  sprintf(dest, "<SF, %s>", get_tag_name(tag).c_str());
  string ret = dest;
  return ret;
}

ID::ID(string name): Token(Tag::ID){
  this->name = name;
}
string ID::inspect(){
  char dest[20];
  sprintf(dest, "<%s, %s>", get_tag_name(Tag::ID).c_str(), name.c_str());
  string ret = dest;
  return ret;
}
Worder::Worder(Reader* reader):Tokener(reader){
  put_reserve("auto", Tag::AUTO);
  put_reserve("break", Tag::BREAK);
  put_reserve("case", Tag::CASE);
  put_reserve("char", Tag::CHAR);
  put_reserve("const", Tag::CONST);
  put_reserve("continue", Tag::CONTINUE);
  put_reserve("default", Tag::DEFAULT);
  put_reserve("do", Tag::DO);
  put_reserve("double", Tag::DOUBLE);
  put_reserve("else", Tag::ELSE);
  put_reserve("enum", Tag::ENUM);
  put_reserve("extern", Tag::EXTERN);
  put_reserve("float", Tag::FLOAT);
  put_reserve("for", Tag::FOR);
  put_reserve("goto", Tag::GOTO);
  put_reserve("if", Tag::IF);
  put_reserve("int", Tag::INT);
  put_reserve("long", Tag::LONG);
  put_reserve("register", Tag::REGISTER);
  put_reserve("return", Tag::RETURN);
  put_reserve("short", Tag::SHORT);
  put_reserve("signed", Tag::SIGNED);
  put_reserve("sizeof", Tag::SIZEOF);
  put_reserve("static", Tag::STATIC);
  put_reserve("struct", Tag::STRUCT);
  put_reserve("switch", Tag::SWITCH);
  put_reserve("typedef", Tag::TYPEDEF);
  put_reserve("union", Tag::UNION);
  put_reserve("unsigned", Tag::UNSIGNED);
  put_reserve("void", Tag::VOID);
  put_reserve("volatile", Tag::VOLATILE);
  put_reserve("while", Tag::WHILE);
}

inline bool Worder::valid_tail(char c){
  return isalpha(c) || isdigit(c) || c=='_';
}

bool Worder::valid_head(char c){
  return isalpha(c) || c=='_';
}

void Worder::put_reserve(string lexeme, Tag tag){
  reserve_words[lexeme] = tag;
}

Token* Worder::forward(){
  Token* ret = nullptr;
  char head;
  while(valid_tail(head = reader->peek())){
    reader->forward();
  }
  string name = reader->confirm();
  map<string, Tag>::iterator iter = reserve_words.find(name);
  if(iter == reserve_words.end()){
    ret = new ID(name);
    ret->lexeme = name;
  }else {
    ret = new SF(reserve_words[name]);
    ret->lexeme = name;
  }
  return ret;
}
