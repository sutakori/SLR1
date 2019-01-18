#include "lstr.h"
#include "error.h"
Lstr::Lstr():Token(Tag::LSTR){}
string Lstr::inspect(){
  char dest[64];
  sprintf(dest, "<%s, %s>", get_tag_name(tag).c_str(), lexeme.c_str());
  string ret = dest;
  return ret;
}
Lstr::~Lstr(){}

Lstrer::Lstrer(Reader* reader):Tokener(reader){}

bool Lstrer::valid_head(char c){
  return c=='"';
}

Token* Lstrer::forward(){
  char head;
  bool escape = false;
  Token* ret;
  Token* errret;
  do {
    reader->forward();
    head = reader->peek();
    if(!escape){
      if(head=='"'){
        reader->forward();
        ret = new Lstr();
        ret->lexeme = reader->confirm();
        return ret;
      }
      if(head=='\n'){
        reader->forward();
        errret = new Err(ErrTag::UNTERMINATED_STRING, reader->getInfo());
        reader->confirm();
        return errret;
      }
      if(head=='\\'){
        escape = true;
      }
    } else {
      // todo: implement escape logic
      escape = false;
    }
  } while(!reader->end);
  errret = new Err(ErrTag::UNTERMINATED_STRING, reader->getInfo());
  reader->confirm();
  return errret;
}
