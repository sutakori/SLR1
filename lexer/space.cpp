#include "space.h"
#include "error.h"
Space::Space():Token(Tag::SPACE){}
string Space::inspect(){
  return "";
}
Space::~Space(){}

Comment::Comment():Token(Tag::COMMENT){}
string Comment::inspect(){
  return "";
}
Comment::~Comment(){}
Spacer::Spacer(Reader* reader):Tokener(reader){}

bool Spacer::valid_head(char c){
  return isspace(c) || c=='/';
}

Token* Spacer::forward(){
  char head = reader->peek();
  // handle space
  if(isspace(head)){
    do{
      reader->forward();
    }while(isspace(head = reader->peek()));
    reader->confirm();
    return new Space();
  }
  // handle comment
  reader->forward();
  if(reader->peek()=='/'){
    do{
      reader->forward();
    }while(reader->peek()!='\n');
    reader->confirm();
    return new Comment();
  }
  if(reader->peek()=='*'){
    bool endready = false;
    do {
      reader->forward();
      head = reader->peek();
      if(endready){
        if(head=='/'){
          reader->forward();
          reader->confirm();
          return new Comment();
        } else endready = false;
      }
      if(head=='*') endready = true;
    } while(!reader->end);
    Token* errret = new Err(ErrTag::UNTERMINATED_COMMENT, reader->getInfo());
    reader->confirm();
    return errret;
  }
  reader->reset();
  return nullptr;
}
