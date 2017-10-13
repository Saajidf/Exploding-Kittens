#include "Player.h"


Player::Player(){

}
  
Player::Player(string name){
  m_name = name;
  vector<Card> m_hand;
  m_lost = false;
  m_extraTurn = false;
}
  
Card Player::PlayCard(int choice){
  
  //will return errorC if no pair
  Card errorC = Card(10,"false");
  // creates temp card = chosen card in hand
  Card newCard = m_hand[choice];
  
  if(newCard.GetType() == 6 or 
     newCard.GetType() == 7 or
     newCard.GetType() == 8 or 
     newCard.GetType() == 9){
    
    m_hand.erase(m_hand.begin()+ choice);
    bool pair = false;
    // if hand contains any cards after attempted first of pair is played then it will search for a match in hand
    if(m_hand.size()>0){
      //if match is found then the match is erased from hand as well as chosen card
      for(unsigned int x = 0; x < m_hand.size(); x++){
	int a = newCard.GetType();
	int b = m_hand[x].GetType();
	if(a == b){
	  m_hand.erase(m_hand.begin()+ x);

	  pair = true;
	  return newCard;
	}
      }
      if(pair == true){
	return newCard;
      }
      // otherwise chosen card is put back and error is returned in order to let user know
      else{
	cout << "Sorry no pairs\n" << endl;
	m_hand.push_back(newCard);
	return errorC; 
      }

    }

    //if hand contains no other cards
    else{
      cout << "Sorry no pairs\n" << endl;
      m_hand.push_back(newCard);
      return errorC;
    }
  }

  //if not a 6-9 card
  else{
    m_hand.erase(m_hand.begin()+ choice);
    return newCard;
  }
}

Card Player::LoseCard(int choice){
  Card card = m_hand[choice];
  m_hand.erase(m_hand.begin()+ choice);
  return card;
}

bool Player::HasCards(){
  // if player hand has more than 0 elements it returns true
  if(m_hand.size() > 0){
    return true;
  }
  else{
    return false;
  }
}
  
bool Player::HasLost(){
  //if - else  returns bool
  if(m_lost == true){
    return true;
  }
  else{
    return false;
  }
}

bool Player::HasExtraTurn(){
  if(m_lost == false){
    return m_extraTurn;
  }
  else{
    return false;
  }
}
  
int Player::AddToHand(Card newCard){
  m_hand.push_back(newCard);
  // adds card to hand
  // returns type of card so that Game.cpp can see if it was a bomb or not
  return newCard.GetType();
}

void Player::SetName(string name){
  m_name = name;
}
  
string Player::GetName(){
  return m_name;
}
  
void Player::DisplayHand(){
  unsigned int size = m_hand.size();
  // for loop to print each card's description in hand
  for(unsigned int i = 0; i < size; i++){

    cout << i << ": "<< m_hand[i].ToString() << endl;
  }
}
  
int Player::GetNumberOfCards(){
  return m_hand.size();
}
  
Card Player::ReturnBomb(){
  Card newCard = m_hand[m_hand.size()-1];
  bool defuse = false;
  // checks if player has a defuse card
  for(unsigned int i = 0; i < m_hand.size(); i++){
    if(m_hand[i].GetType() == 1){
      defuse = true;
      newCard = m_hand[i];
      //if they did then bomb is erased and difuse is passed in order to is to be discarded
      m_hand.erase(m_hand.begin()+(m_hand.size()-1));
      
      m_hand.erase(m_hand.begin()+i);
      return newCard;
    }
  }
  //if no diffuse then bomb will be passed back
  return newCard;
}
  
void Player::SetLost(bool set){
  m_lost = set;
}
  
void Player::SetExtraTurn(bool set){
  m_extraTurn = set;
}
