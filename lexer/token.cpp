#include "token.h"

Tokener::Tokener(Reader* reader){
  this->reader = reader;
}

Tokener::~Tokener(){
  delete reader; reader = 0;
}
