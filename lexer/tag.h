#ifndef LEXER_TAG
#define LEXER_TAG

#include <string>
using std::string;

enum class Tag
{
  ERR,
  ID,

  SPACE,
  COMMENT,
  LSTR,

  INTEGER,
  REAL,

  TRUE,
  FALSE,
  // operators
  ASSIGN,    // =
  ADDASSIGN, // +=
  SUBASSIGN, // -=
  MULASSIGN, // *=
  DIVASSIGN, // /=
  MODASSIGN, // %=
  LSASSIGN,  // <<=
  RSASSIGN,  // >>=
  ANDASSIGN, // &=
  EDASSIGN,  // ^=
  ORASSIGN,  // |=

  ADDADD,  // ++
  SUBSUB,    // --

  ADD,       // +
  SUB,       // -
  MUL,       // *
  DIV,       // /
  MOD,       // %
  LS,        // <<
  RS,        // >>
  UNIAND,    // &
  BITED,     // ^
  BITOR,     // |
  BITNOT,    // ~

  EQ,        // ==
  NE,        // !=
  LT,        // <
  GT,        // >
  LE,        // <=
  GE,        // >=

  AND,       // &&
  OR,        // ||
  NOT,       // !

  SPL,       // (
  SPR,       // )
  MPL,       // [
  MPR,       // ]
  LPL,       // {
  LPR,       // }
  COMMA,     // ,
  SEMICOLON, // ;
  COLON,     // :
  CONDITION, // ?

  POINT,     // .
  ARROW,     // ->

  AUTO,
  BREAK,
  CASE,
  CHAR,
  CONST,
  CONTINUE,
  DEFAULT,
  DO,
  DOUBLE,
  ELSE,
  ENUM,
  EXTERN,
  FLOAT,
  FOR,
  GOTO,
  IF,
  INT,
  LONG,
  REGISTER,
  RETURN,
  SHORT,
  SIGNED,
  SIZEOF,
  STATIC,
  STRUCT,
  SWITCH,
  TYPEDEF,
  UNION,
  UNSIGNED,
  VOID,
  VOLATILE,
  WHILE,

  PARSEEND
};

extern string get_tag_name(Tag tag);
#endif
