#ifndef PARSER_H
#define PARSER_H

#include <stack>
#include <map>
#include <vector>
#include <tuple>
#include <memory>
#include "../lexer/token.h"
#include "../lexer/num.h"
#include "../lexer/tag.h"
using namespace std;


class Symbol;
class Nonterminate{
public:
  string name;
public:
  Nonterminate(string name){this->name=name;}
  Nonterminate* clone(){return new Nonterminate(this->name);}
  string identification(){return name;}
};

class Symbol{
public:
  Symbol(Nonterminate* non);
  Symbol(Token* token);
  vector<Symbol*> childs;
  bool istoken(){return tag==0;}
public:
  int tag; // 0 terminate; 1 non
  union {
    Token* token;
    Nonterminate* non;
  };
};

class Grammar{
public:
  Nonterminate* lhs;
  vector<Symbol*> rhs;
public:
  Grammar(Nonterminate* lhs, vector<Symbol*> symbols){
    this->lhs = lhs;
    rhs = move(symbols);
  }
  int lenRHS(){return rhs.size();}
  Nonterminate* getLHS(){return lhs;}
  bool matchlhs(Symbol* sym){return sym->non->name==lhs->name;}
};
enum class ActionTyp
  {
   shift,
   reduce,
   accept,
   error
  };

class Action{
public:
  ActionTyp typ;
  void* act;
  Action(ActionTyp typ,void* act){
    this->typ=typ;
    this->act=act;
  }
};

class CFSMNode{
public:
  vector<Grammar*> grammars;
  vector<int> curs;
  map<Tag, CFSMNode*> shiftToken;
  map<string, CFSMNode*> shiftNon;

  bool equal(CFSMNode*);
};

class CFSM{
public:
  ~CFSM();
  CFSMNode* root;
  vector<CFSMNode*> nodes;

  CFSMNode* find(CFSMNode*);
};

class Parser{
private:
  vector<Grammar*> _grammars;
  vector<Tag> _tokens;
  vector<string> _nons;
  map<CFSMNode*, map<Tag, Action>> _action;
  map<CFSMNode*, map<string, CFSMNode*>> _goto;
  map<string, vector<Tag>> _first;
  map<string, vector<Tag>> _follow;
  CFSM _cfsm;
public:
  Parser();
  Symbol* parse(vector<Token*> tokens);
  void printCFSM();
  void printTable();
  void view_tree(Symbol*, int deep=0);
  ~Parser();
private:
  vector<Tag> compute_first(vector<Symbol*>);
  void generate_first();
  void generate_follow();
  void generate_table();
  void generate_cfsm();
  void closure(CFSMNode*);
  void go(CFSMNode*);
};

#endif
