#include "operator.h"
#include "tag.h"
#include "error.h"

OP::OP(Tag tag):Token(tag){}
string OP::inspect(){
  char dest[20];
  sprintf(dest, "<%s>", get_tag_name(tag).c_str());
  string ret = dest;
  return ret;
}
OP::~OP(){}
OPer::OPer(Reader* reader):Tokener(reader){}

bool OPer::valid_head(char c){
  return valid_heads.find(c) != string::npos;
}

Token* OPer::forward(){
  char head = reader->peek();
  reader->forward();
  char next = reader->peek();
  Token* ret;
  switch(head){
  case '+':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::ADDASSIGN); break;
    } else if(next == '+') {
      reader->forward(); ret = new OP(Tag::ADDADD); break;
    }ret = new OP(Tag::ADD); break;
  case '-':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::SUBASSIGN); break;
    } else if(next == '-') {
      reader->forward(); ret = new OP(Tag::SUBSUB); break;
    } ret = new OP(Tag::SUB); break;
  case '*':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::MULASSIGN); break;
    } else ret = new OP(Tag::MUL); break;
  case '/':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::DIVASSIGN); break;
    } else ret = new OP(Tag::DIV); break;
  case '%':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::MODASSIGN); break;
    } else ret = new OP(Tag::MOD); break;
  case '&':
    if(next == '&') {
      reader->forward(); ret = new OP(Tag::AND); break;
    } else ret = new OP(Tag::UNIAND); break;
  case '^':
    ret = new OP(Tag::BITED); break;
  case '|':
    if(next == '|') {
      reader->forward(); ret = new OP(Tag::OR); break;
    } else ret = new OP(Tag::BITOR); break;
  case '~':
      reader->forward(); ret = new OP(Tag::BITNOT); break;
  case '!':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::NE); break;
    } else ret = new OP(Tag::NOT); break;
  case '(':
    ret = new OP(Tag::SPL); break;
  case ')':
    ret = new OP(Tag::SPR); break;
  case '[':
    ret = new OP(Tag::MPL); break;
  case ']':
    ret = new OP(Tag::MPR); break;
  case '{':
    ret = new OP(Tag::LPL); break;
  case '}':
    ret = new OP(Tag::LPR); break;
  case '?':
    ret = new OP(Tag::CONDITION); break;
  case '.':
    ret = new OP(Tag::POINT); break;
  case ',':
    ret = new OP(Tag::COMMA); break;
  case ';':
    ret = new OP(Tag::SEMICOLON); break;
  case ':':
    ret = new OP(Tag::COLON); break;
  case '=':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::EQ); break;
    } else ret = new OP(Tag::ASSIGN); break;
  case '<':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::LE); break;
    }
    if(next == '<') {
      reader->forward();
      next = reader->peek();
      if(next == '=') {
        reader->forward();
        ret = new OP(Tag::LSASSIGN); break;
      } else {
        ret = new OP(Tag::LS); break;
      }
    } else ret = new OP(Tag::LT); break;
  case '>':
    if(next == '=') {
      reader->forward(); ret = new OP(Tag::GE); break;
    }
    if(next == '>') {
      reader->forward();
      next = reader->peek();
      if(next == '=') {
        reader->forward();
        ret = new OP(Tag::RSASSIGN); break;
      } else {
        ret = new OP(Tag::RS); break;
      }
    } else ret = new OP(Tag::GT); break;
  }
  ret->lexeme = reader->confirm();
  return ret;
}
