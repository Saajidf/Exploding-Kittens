#include "Card.h"

Card::Card(){
  m_type = 0;
  m_desc = "";
}

Card::Card(int a, string b){
  m_type = a;
  m_desc = b;
}

int Card::GetType(){
  return m_type;
}

string Card::ToString(){
  return m_desc;
}
