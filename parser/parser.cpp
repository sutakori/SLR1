#include "parser.h"
#include "tokens.h"
#include "../lexer/tag.h"
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
using namespace std;

Symbol::Symbol(Nonterminate* non){
  tag = 1;
  this->non = non;
}
Symbol::Symbol(Token* token){
  tag = 0;
  this->token = token;
}

Parser::Parser(){
  _grammars.push_back(new Grammar(new Nonterminate("S"), vector<Symbol*>{
        new Symbol(new Nonterminate("E")),
      }));
  _grammars.push_back(new Grammar(new Nonterminate("E"), vector<Symbol*>{
        new Symbol(new Nonterminate("E")),
          new Symbol(new OP(Tag::ADD)),
          new Symbol(new Nonterminate("T"))
      }));
  _grammars.push_back(new Grammar(new Nonterminate("E"), vector<Symbol*>{
        new Symbol(new Nonterminate("E")),
          new Symbol(new OP(Tag::SUB)),
          new Symbol(new Nonterminate("T"))
      }));
  _grammars.push_back(new Grammar(new Nonterminate("E"), vector<Symbol*>{
          new Symbol(new Nonterminate("T"))
      }));
  _grammars.push_back(new Grammar(new Nonterminate("T"), vector<Symbol*>{
        new Symbol(new Nonterminate("T")),
          new Symbol(new OP(Tag::MUL)),
          new Symbol(new Nonterminate("F"))
      }));
  _grammars.push_back(new Grammar(new Nonterminate("T"), vector<Symbol*>{
        new Symbol(new Nonterminate("T")),
          new Symbol(new OP(Tag::DIV)),
          new Symbol(new Nonterminate("F"))
      }));
  _grammars.push_back(new Grammar(new Nonterminate("T"), vector<Symbol*>{
          new Symbol(new Nonterminate("F"))
      }));
  _grammars.push_back(new Grammar(new Nonterminate("F"), vector<Symbol*>{
        new Symbol(new OP(Tag::SPL)),
          new Symbol(new Nonterminate("E")),
          new Symbol(new OP(Tag::SPR))
      }));
  _grammars.push_back(new Grammar(new Nonterminate("F"), vector<Symbol*>{
        new Symbol(new Int(Tag::INTEGER, 0)),
      }));
  _tokens = vector<Tag>{Tag::ADD,Tag::SUB,Tag::MUL,Tag::DIV,Tag::SPL,Tag::SPR,Tag::INTEGER};
  _nons = vector<string>{"S","E","T","F"};
  generate_first();
  generate_follow();
  generate_cfsm();
  generate_table();
  printCFSM();
  printTable();
}

vector<Tag> Parser::compute_first(vector<Symbol *> syms){
  vector<Tag> firsts;
  auto f = syms[0];
  if(f->istoken())
    firsts = vector<Tag>{f->token->tag};
  else{
    if(_first.count(f->non->name)>0){
      firsts = _first[f->non->name];
    }else{
      firsts = vector<Tag>{};
    }
  }
  return firsts;
}

void Parser::generate_first(){
  bool changed;
  do{
    changed=false;
    for(auto g : _grammars){
      vector<Tag> firsts = compute_first(g->rhs);
      for(auto tag : firsts){
        if(find(_first[g->lhs->name].begin(), _first[g->lhs->name].end(), tag)==_first[g->lhs->name].end()){
          _first[g->lhs->name].push_back(tag);
          changed=true;
        }
      }
    }
  }while(changed);
}
void Parser::generate_follow(){
  _follow[_grammars[0]->lhs->name] = vector<Tag>{Tag::PARSEEND};
  bool changed;
  do{
    changed=false;
    for(auto g : _grammars){
      for(auto is=g->rhs.begin();is!=g->rhs.end();++is){
        if(!(*is)->istoken()){
          vector<Symbol*> fetch_first;
          fetch_first.insert(fetch_first.end(), is+1, g->rhs.end());
          vector<Tag> firsts;
          if(fetch_first.size()>0)
            firsts = compute_first(fetch_first);
          else
            if(_follow.count(g->lhs->name)>0){
              firsts = _follow[g->lhs->name];
            }else{
              firsts = vector<Tag>{};
            }
          for(auto tag : firsts){
            if(find(_follow[(*is)->non->name].begin(), _follow[(*is)->non->name].end(), tag)==_follow[(*is)->non->name].end()){
              _follow[(*is)->non->name].push_back(tag);
              changed=true;
            }
          }
        }
      }
    }
  }while(changed);
}
bool CFSMNode::equal(CFSMNode* cur){
  if(grammars.size()!=cur->grammars.size()) return false;
  for(int i=0; i<int(grammars.size()); i++){
    if(grammars[i]!=cur->grammars[i] or curs[i]!=cur->curs[i]){
      return false;
    }
  }
  return true;
}

CFSMNode* CFSM::find(CFSMNode* cur){
  for(auto node:nodes){
    if(node->equal(cur)){
      return node;
    }
  }
  return nullptr;
}

void Parser::closure(CFSMNode * node){
  bool changed=false;
  do{
    changed=false;
    int curlen=int(node->grammars.size());
    for(int i=0;i<curlen;i++){
      auto grammar = (node->grammars)[i];
      if(node->curs[i]==int(grammar->rhs.size())) continue;
      Symbol* sym = grammar->rhs[node->curs[i]];
      if(sym and !sym->istoken()){
        for(auto ig=_grammars.begin();ig!=_grammars.end();++ig){
          if((*ig)->matchlhs(sym)){
            int j=0;
            for(;j<int(node->grammars.size());j++){
              if(node->grammars[j]==*ig and node->curs[j]==0) break;
            }
            if(j==int(node->grammars.size())){
              node->grammars.push_back(*ig);
              node->curs.push_back(0);
              changed=true;
            }
          }
        }
      }
    }
  }while(changed);
}

void Parser::go(CFSMNode* node){
  for(int i=0;i<int(node->grammars.size());i++){
    Symbol* sym = node->grammars[i]->rhs[node->curs[i]];
    if(node->curs[i]==int(node->grammars[i]->rhs.size())) continue;
    if(sym and sym->istoken()){
      if(node->shiftToken.count(sym->token->tag)==0){
        // go by the symbol
        CFSMNode* next = new CFSMNode();
        next->grammars.push_back(node->grammars[i]);
        next->curs.push_back(node->curs[i]+1);
        node->shiftToken[sym->token->tag] = next;
      }else{
        node->shiftToken[sym->token->tag]->grammars.push_back(node->grammars[i]);
        node->shiftToken[sym->token->tag]->curs.push_back(node->curs[i]+1);
      }
    }else if(sym){
      if(node->shiftNon.count(sym->non->name)==0){
        // go by the symbol
        CFSMNode* next = new CFSMNode();
        next->grammars.push_back(node->grammars[i]);
        next->curs.push_back(node->curs[i]+1);
        node->shiftNon[sym->non->name] = next;
      }else{
        node->shiftNon[sym->non->name]->grammars.push_back(node->grammars[i]);
        node->shiftNon[sym->non->name]->curs.push_back(node->curs[i]+1);
      }
    }
  }
  for(auto s:node->shiftNon){
    closure(s.second);
    CFSMNode* already = _cfsm.find(s.second);
    if(already !=  nullptr){
      node->shiftNon[s.first] = already;
    }else{
      _cfsm.nodes.push_back(s.second);
    }

  }
  for(auto s:node->shiftToken){
    closure(s.second);
    CFSMNode* already = _cfsm.find(s.second);
    if(already !=  nullptr){
      node->shiftToken[s.first] = already;
    }else{
      _cfsm.nodes.push_back(s.second);
    }
  }
}

string get_typ_name(ActionTyp tag) {
  switch (tag) {
  case ActionTyp::shift: return "shift";
  case ActionTyp::reduce: return "reduce";
  case ActionTyp::accept: return "accept";
  case ActionTyp::error: return "error";
  }
}

void Parser::generate_cfsm(){
  _cfsm.root = new CFSMNode();
  _cfsm.root->grammars.push_back(_grammars[0]);
  _cfsm.root->curs.push_back(0);
  _cfsm.nodes.push_back(_cfsm.root);
  closure(_cfsm.root);
  for(int i=0;i<int(_cfsm.nodes.size());i++){
    go(_cfsm.nodes[i]);
  }
}
void Parser::generate_table(){
  for(int state=0; state<int(_cfsm.nodes.size()); state++){
    auto node = _cfsm.nodes[state];
    //action
    for(int ig = 0; ig<int(node->grammars.size()); ig++){
      auto g = node->grammars[ig];
      auto cur = node->curs[ig];
      //reduce or accept
      if(int(cur)==g->lenRHS()){
        //accept
        if(g->lhs->name==_grammars[0]->lhs->name){
          _action[_cfsm.nodes[state]].insert(pair<Tag, Action>(Tag::PARSEEND, Action(ActionTyp::accept, (void*)0)));
          continue;
        }
        for(Tag tag: _follow[g->lhs->name]){
          _action[_cfsm.nodes[state]].insert(pair<Tag, Action>(tag, Action(ActionTyp::reduce, node->grammars[ig])));
        }
        continue;
      }
      if(g->rhs[cur]->istoken()){
        auto tag=g->rhs[cur]->token->tag;
        _action[node].insert(pair<Tag, Action>(tag, Action(ActionTyp::shift, node->shiftToken[tag])));
      }else{
        //goto
        _goto[node][g->rhs[cur]->non->name]=node->shiftNon[g->rhs[cur]->non->name];
      }
    }

  }
  return;
}

void Parser::printCFSM(){
  for(auto node : _cfsm.nodes){
    cout<<node<<endl;
    for(int i=0;i<int(node->grammars.size());i++){
      cout<<node->curs[i]<<' '<<node->grammars[i]->lhs->name<<" ->";
      for (auto n : node->grammars[i]->rhs){
        if(n->istoken()){
          cout<<' '<<get_tag_name(n->token->tag);
        }
        else{
          cout<<' '<<n->non->name;
        }
      }
      cout<<endl;
    }
    cout<<endl;
  }
}

void Parser::printTable(){
  cout<<"action:"<<endl;
  for(auto s:_action){
    cout<<s.first<<endl;
    for(auto t:s.second){
      cout<< get_tag_name(t.first)<<" -> "<< get_typ_name(t.second.typ)<<" "<<t.second.act<<endl;
    }
  }
  cout<<"goto:"<<endl;
  for(auto s:_goto){
    cout<<s.first<<endl;
    for(auto t:s.second){
      cout<<t.first<<" -> "<<t.second<<endl;
    }
  }
}

Symbol* Parser::parse(vector<Token*> tokens){
  Token* token;
  tokens.push_back(new OP(Tag::PARSEEND));
  CFSMNode* state = _cfsm.root;
  auto it=tokens.begin();
  stack<Symbol*> parseStack;
  stack<CFSMNode*> stateStack;
  stack<Symbol*> temp;
  stack<Symbol*> temp2;
  parseStack.push(new Symbol(new OP(Tag::PARSEEND)));
  stateStack.push(state);

  while(true){
    cout<<"state "<<state<<endl;
    if (it==tokens.end()) break;

    //print
    temp = parseStack;
    while(!temp.empty()){
      Symbol* sym=temp.top();
      temp2.push(sym);
      temp.pop();
    }
    while(!temp2.empty()){
      Symbol* sym=temp2.top();
      if(sym->istoken()){
        cout<<' '<< sym->token->inspect();
      }else{
        cout<<' '<<sym->non->name;
      }
      temp2.pop();
    }
    cout<<"--------------";
    vector<Token*> rest;
    rest.insert(rest.begin(),it,tokens.end());
    for(auto t:rest){
      cout<<t->inspect()<<' ';
    }
    rest.erase(rest.begin(), rest.end());
    cout<<endl;
    //

    token = *it;
    Action action(ActionTyp::error, (void*)0);
    if(_action[state].count(token->getTag())>0)
      action = _action[state].at(token->getTag());
    if(action.typ==ActionTyp::shift){
      cout<<"shift ";
      state = (CFSMNode*)action.act;
      parseStack.push(new Symbol(token));
      stateStack.push(state);
      ++it;
    }else if(action.typ==ActionTyp::reduce){
      cout<<"reduce ";
      Grammar* grammar = (Grammar*)action.act;
      auto lhs = grammar->getLHS();
      Symbol* node = new Symbol(lhs->clone());
      for(int i=0;i<grammar->lenRHS();i++){
        node->childs.push_back(parseStack.top());
        parseStack.pop();
        stateStack.pop();
      }
      reverse(node->childs.begin(), node->childs.end());
      state = _goto[stateStack.top()][lhs->name];
      parseStack.push(node);
      stateStack.push(state);
    }else if(action.typ==ActionTyp::accept){
      cout<<"accept"<<endl<<endl;
      return parseStack.top();
    }else{
      cout<<"parse error"<<endl;
      return nullptr;
    }
  }
}

void Parser::view_tree(Symbol* root, int deep){
  int w=2;
  string space(deep*w, ' ');
  if(root->istoken()){
    cout<<space<<root->token->inspect()<<endl;
  }else{
    cout<<space<<root->non->name<<endl;
    for(auto c:root->childs){
      view_tree(c, deep+1);
    }
  }
}

CFSM::~CFSM(){
  for_each(nodes.begin(), nodes.end(), [] (CFSMNode* p) -> void {delete p;} );
}
