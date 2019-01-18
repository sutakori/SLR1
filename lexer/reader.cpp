#include <stdio.h>
#include "reader.h"

Reader::Reader(string filepath){
  pSource = fopen(filepath.c_str(), "rb");
  inbuf[bufend1] = EOF;
  inbuf[bufend2] = EOF;
  fetchbuf(1);
  fetchbuf(2);
  cur = 0;
  end = false;

  info.filename = filepath;
  info.linenum = 1;
  info.startLinenum = 1;
  info.startcol = 1;
  info.traceflag = 1;
  info.tracecol = 0;
  info.curcol = 1;
  info.line = inbuf[cur]+searchLine();
  info.startline = info.line;
  info.tokenbuf = "";
  oldinfo = info;
}

Reader::~Reader(){
  if(pSource){
    fclose(pSource);
  }
}

void Reader::fetchbuf(int n){
  int len;
  if(n==1){
    len = fread(inbuf, sizeof(char),bufsize-1, pSource);
    if(len!=bufsize-1){
      inbuf[len]=EOF;
    }
  }else{
    len = fread(inbuf+bufsize, sizeof(char), bufsize-1, pSource);
    if(len!=bufsize-1){
      inbuf[bufsize+len]=EOF;
    }
  }
}

void Reader::forward(){
  info.tokenbuf += inbuf[cur];
  cur++;
  info.curcol++;
  if(inbuf[cur]==EOF){
    switch(cur){
    case bufend1:
      cur++; fetchbuf(1); break;
    case bufend2:
      cur=0; fetchbuf(2); break;
    default:
      end = true; return;
    }
    return;
  }
  if(inbuf[cur]=='\n'){
    info.linenum++;
    info.curcol = 1;
    info.line = searchLine();
    info.traceflag = 0;
    if(info.traceflag){
      info.startline = info.line;
      info.startLinenum = info.linenum;
    }
    return;
  }
  if(info.traceflag) info.tracecol++;
}

string Reader::searchLine(){
  int p = cur;
  string line="";
  do{
    p++;
    switch(inbuf[p]){
    case EOF:
      switch(p){
      case bufend1:
        p++; break;
      case bufend2:
        p=-1; break;
      default:
        return line;
      }
      break;
    case '\n':
      return line;
    default:
      line += inbuf[p];
    }
  }while(p!=cur);
  return line;
}

char Reader::peek(){
  return inbuf[cur];
}

string Reader::confirm(){
  string ret = info.tokenbuf;
  info.startcol = info.curcol;
  info.tokenbuf = "";
  info.startline = info.line;
  info.startLinenum = info.linenum;
  info.tracecol = 0;
  info.traceflag = 1;
  oldinfo = info;
  start=cur;
  return ret;
}

int Reader::inWhichBuf(int p){
  if(p<=bufend1){
    return 1;
  }else return 2;
}

void Reader::reset(){
  info = oldinfo;
  if(inWhichBuf(cur) != inWhichBuf(start)){
    fseek(pSource, -Reader::bufsize+1, SEEK_CUR);
    fetchbuf(inWhichBuf(start));
  }
  cur = start;
}

ReadInfo Reader::getInfo(){
  return info;
}
