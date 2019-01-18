#ifndef LEXER_READER
#define LEXER_READER

#include <string>
using std::string;

struct ReadInfo{
  string filename;
  string tokenbuf;
  int linenum;
  int curcol;
  string line;

  int startcol;
  int startLinenum;
  string startline;
  int traceflag;
  int tracecol;
};

class Reader{
  /* when tokener is ready to fetch a new token
   * tokenbuf = ""
   * start and cur point to the first char of the new token
   */
public:
  Reader(string filepath);
  ~Reader();

  /* move forward and update line info
   * pre char is gatherd as the last char of the last token
   */
  void forward();

  /* return the current char
   */
  char peek();
  /* abort a token search
   *  state of the reader is reset to the
   */
  void reset();

  /* return the lexeme
   * before called, cur point to the 1 off the last char
   */
  string confirm();
  ReadInfo getInfo();
  void fetchbuf(int n);
  bool end;
  static const int bufsize = 64;
  static const int bufend1 = bufsize-1;
  static const int bufend2 = 2*bufsize - 1;
private:
  FILE* pSource;
  char inbuf[2* bufsize];
  int inWhichBuf(int p);
  int start;
  int cur;

  string searchLine();
  ReadInfo oldinfo; //info at last read token
  ReadInfo info;
};

#endif
