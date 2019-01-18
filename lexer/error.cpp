#include "error.h"

Err::Err(ErrTag err, ReadInfo info):Token(Tag::ERR){
  this->errtag = err;
  this->info = info;
}
Err::~Err(){}

string Err::inspect(){
  char err_report[128];
  string indicator(info.startcol-2, ' ');
  indicator += '^';
  string trace(info.tracecol, '~');
  indicator += trace;
  sprintf(err_report,
          "\n%s:%d:%d: lexer error: %s\n%s\n%s\n",
          info.filename.c_str(), info.startLinenum, info.startcol-1,
          errIndi().c_str(), info.startline.c_str(), indicator.c_str());
  string ret = err_report;
  return ret;
}

string Err::errIndi(){
  switch(errtag){
  case ErrTag::STRAY_CHAR:
    return "stary char "+info.tokenbuf;
  case ErrTag::UNTERMINATED_COMMENT:
    return "unterminated comment";
  case ErrTag::UNTERMINATED_STRING:
    return "unterminated string";
  }
}
