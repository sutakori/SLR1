#include <string>
#include <iostream>
#include "token.h"
#include "error.h"
#include "lexer.h"
#include "operator.h"
#include "num.h"
#include "reader.h"
#include "lstr.h"
#include "space.h"

using std::string;
using std::cout;
using std::endl;

Lexer::Lexer(string filepath){
  reader = new Reader(filepath);
  numer = new Numer(reader);
  oper = new OPer(reader);
  worder = new Worder(reader);
  spacer = new Spacer(reader);
  lstrer = new Lstrer(reader);

  tokens.reserve(40);
}

Lexer::~Lexer(){
  delete numer; numer = 0;
  delete oper; oper = 0;
  delete worder; worder = 0;
  delete reader; reader = 0;
  delete spacer; spacer = 0;
  delete lstrer; lstrer = 0;

  for (vector<Token*>::iterator it = tokens.begin(); it != tokens.end(); it ++)
    if (NULL != *it)
      {
        delete *it;
        *it = NULL;
      }
  tokens.clear();
}

Token* Lexer::forward(){
  char head = reader->peek();
  if (head == 0) return nullptr;

  Token* next = nullptr;
  if(spacer->valid_head(head)){
    next = spacer->forward();
    if(next)
      return next;
  }
  if(lstrer->valid_head(head)){
    next = lstrer->forward();
    return next;
  }
  // number
  if(numer->valid_head(head)){
    next = numer->forward();
    return next;
  }
  // operators
  if(oper->valid_head(head)){
    next = oper->forward();
    return next;
  }
  // words
  if(worder->valid_head(head)){
    next = worder->forward();
    return next;
  }

  // error
  if(reader->end) return nullptr;
  next = new Err(ErrTag::STRAY_CHAR, reader->getInfo());
  reader->forward();
  reader->confirm();
  return next;
}

int Lexer::scan(){
  Token* next;
  while((next=forward())){
    switch(next->tag){
    case Tag::ERR:
      cout << next->inspect(); break;
    case Tag::SPACE:
    case Tag::COMMENT:
      break;
    default:
      tokens.push_back(next);
    }
  }
  return 0;
}

vector<Token*> Lexer::getTokens(){
  return this->tokens;
}

void Lexer::print(){
  for(auto token : tokens){
    cout << token->inspect() << "  ";
  }
}
