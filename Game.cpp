#include <fstream>
#include "Game.h"

Game::Game(){
  int userInput = 0;
  int PLAYER_RANGE[2] = {2,4};

  //prints intro
  cout << "\nMEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOW" << endl <<
    "Welcome to Exploding Kittens!" << endl <<
    "A Game By Elan Lee, Shane Small, and Matthew Inman" << endl <<
    "MEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOWMEOW\n" << endl;
  
  cout << "How many players would you like?" << endl;
  //gets valid int within given menu range
  userInput = getValidInt(PLAYER_RANGE);
  //sets num of players as well and naming them
  SetNumPlayers(userInput);

  LoadDeck(DECK_NAME);
  Shuffle();
  cout << "\nDeck has been shuffled." << endl << "Dealing" << endl;
  
  /* DEAL CARDS HERE
  loops until last player has 4 cards which is initial num of cards in hand  
  adds first element of deck to each players hand then deletes from deck after every add*/
  for(int j = 0; j < INIT_CARDS; j++){
    for(int i = 0; i < m_numPlayers; i++){
      m_players[i].AddToHand(m_deck[0]);
      m_deck.erase(m_deck.begin());
    }
  }

  AddBombCards(BOMB_NAME);
  Shuffle();
  cout << "Shuffling in exploding kittens." << endl;

  PlayGame();

  //prints out winner at end
  for(int x = 0; x < m_numPlayers; x++){
    if(m_players[x].HasLost() == false){
      cout << "Player " << x << " wins" << endl;
    }
  }


}

int Game::getValidInt(int range[]){
  int result = 0;
  // do while loop so that user may enter an input at least once and they will keep inputing until they give a valid input
  do{
    cout << "Please input a number between " << range[0] << " and " <<
      range[1] << ":" << endl;
    cin >> result;

  }while((result < range[0]) or (result > range[1]));

  return result;
}

void Game::SetNumPlayers(int numPlayers){
  //sets num players
  m_numPlayers = numPlayers;

  for(int i = 0; i < m_numPlayers; i++){
    //coverts int to string inorder to name each player
    stringstream ss;
    ss << (i + 1);
    string str = ss.str();
    m_players[i].SetName(str);
  }
}

void Game::Shuffle(){
  unsigned int size = m_deck.size();
  Card temp;
  // creates the random seed
  srand(RAND_SEED);

  //loops through deck and replaces each iteration with a random other card in deck
  //to do this a copy of original is made to temp variable so that random card may take spot of iteration
  for(unsigned int i = 0; i < size; i++){
    unsigned int r = i + rand() % (size - i);
    temp = m_deck[i];
    m_deck[i] = m_deck[r];
    m_deck[r] = temp;
  }
   
}

void Game::Peek(){

  //if deck has at least one card then the can peek
  if(m_deck.size() > 0){
    //for loop will peek at top 3 or rest of deck depending on which is smaller
    int look = ((PEEK_NUM <= m_deck.size()) ? PEEK_NUM : m_deck.size());
    for(int i = 0; i < look; i++){
      cout << m_deck[i].ToString() << endl;
    }
  }
  //if deck is empty then cannot peek
  else{
    cout << "Sorry deck is empty\n" << endl;
  }
}

Card Game::DrawCard(){
  // create new card object and sets it equall to top of deck then top of deck get erased
  Card newCard = m_deck[0];
  m_deck.erase(m_deck.begin());
  //returns copy of top of deck
  return newCard;
}

int Game::HandleCard(int player){
  
  int range[] = {0, (m_players[player].GetNumberOfCards()-1)};

  int choice = getValidInt(range);
  Card Pick = m_players[player].PlayCard(choice);//sets pick equal to card chosen as well as removing card form player's hand
  //if card chosen was a pair and no pairs were found then error card with type  10 will be returned 

  //switch statement to use card depending on type
  switch(Pick.GetType()){
  cout << "\nYou have played: " << Pick.ToString() << endl;

  case 1://defuse does nothing if played so puts card back in player hand
    m_players[player].AddToHand(Pick);
    cout << "Sorry you cannot defuse right now!\n" << endl;

    break;

  case 2://Attack
    cout << "You are attacking the next player and ending turn" << endl;
    //sets next players turn to 2
    if(player == m_numPlayers){
      m_players[0].SetExtraTurn(true);
    }
    else{
      m_players[player+1].SetExtraTurn(true);
    }

    m_discard.push_back(Pick); //discards card after played
    return -1;
    break;
 
  case 3://Skip
    m_discard.push_back(Pick); //discards card after played
    cout << "Skip card used, so turn ends\n" << endl;
    return -1;//this will return -1 which is assigned to end player turn before drawing
    break;

  case 4://Peek
    cout << "Top cards of the deck\n" << endl;
    Peek(); //looks at top of deck
    cout << "\n" << endl;
    m_discard.push_back(Pick); //discards card after played
    break;

  case 5://Shuffle
    cout <<"Shuffling deck\n"<< endl;
    Shuffle(); //shuffles deck
    m_discard.push_back(Pick); //discards card after played
    break;

  case 6:
  case 7:
  case 8:
  case 9://case 6-9 if successfull pair, steals random card from next player
    srand(RAND_SEED);

    if(player == m_numPlayers){
      //gets random num based on next players cards in hand
      int randNum = rand() % m_players[0].GetNumberOfCards();
 
      Card Steal = m_players[0].LoseCard(randNum);
      m_players[player].AddToHand(Steal);
    }
    else{

      int randNum = rand() % m_players[player+1].GetNumberOfCards();

      Card Steal = m_players[player+1].LoseCard(randNum);
      m_players[player].AddToHand(Steal);
    }


    cout << "You Have stolen a card!" << endl;

    break;

  case 10://if fail pair
    cout << "You cannot play that card by itself.\n" << endl;
    break;
  }
  
  return 0;// will not end turn after returning to PlayGame()
}

void Game::PlayGame(){

  //gameOver will be numPlayers - 1 
  int gameOver = m_numPlayers - 1;
  int lostPlayers = 0;

  //for some reason every player would start out with an extra turn so this bit of code is here to fix that error
  for(int z = 0; z < m_numPlayers; z++){
    m_players[z].SetExtraTurn(false);
  }



  //this is range of what a player can choose from menu
  int MENU_RANGE[2] = {1,3};
  int choice = 0;

  //iterator of player turns
  int i = 0;



  //will run until lostPlayers == gameOver
  while(lostPlayers < gameOver){
 
    //keeps track of when turn is over
    bool currentTurn = true;

    //skips turn if current player has lost
    if(m_players[i].HasLost() == true){
      currentTurn == false;
    }
    
    //runs until currentTurn == false
    while(currentTurn == true){


      //if player at i has cards they may choose from menu 
      if(m_players[i].HasCards() == true){
	cout << "\nNum cards remaining: " << m_deck.size() << "\n" << endl;
	
	cout << "It is Player " << m_players[i].GetName() << "'s turn" << endl;
	cout << "What would you like to do: " << endl <<
	  "1. View Cards" << endl <<
	  "2. Play A Card" << endl <<
	  "3. Draw A Card\n" << endl;
	
	choice = getValidInt(MENU_RANGE);
	
	//switch statement for menu choice
	switch(choice){
	  //prints out hand
	case 1:
	  cout << "This is your hand:\n" << endl;
	  m_players[i].DisplayHand();
	  break;
	  //prints hand and then asks which card user wishes to play
	case 2:
	  cout << "You may play a card:\n" << endl;
	  m_players[i].DisplayHand();
	  
	  //if their card has a skip added to it then -1 will be returned in order for player to not draw that turn
	  if(HandleCard(i) == -1){
	    if(i == m_numPlayers){
	      i = 0;
	    }
	    else{
	      i++;
	    }
	  }	  
	  else{
	    currentTurn == true;
	  }
	  break;

	  //if they draw and its a bomb this nested if else will check if they have a diffuse
	  //otherwise player will lose and lostPlayers will iterate
	case 3:
	  int bomb = m_players[i].AddToHand(DrawCard());
	  if(bomb == 0){
	    cout << "You have drawn a bomb!!!" << endl;
	    //return bomb will either return bomb or return defuse if return defuse new bomb will be placed in deck
	    Card newCard = m_players[i].ReturnBomb();

	    if(newCard.GetType() == 0){
	      m_players[i].SetLost(true);
	      //adds one to lostPlayers
	      lostPlayers++;
	      cout << "Player " << m_players[i].GetName() << " has failed to defuse bomb. Player " << m_players[i].GetName() << " has lost.\n" << endl; 
	    }

	    else{
	      //discards diffuse
	      m_discard.push_back(newCard);
	      cout << "You have diffused the bomb" << endl;
	      //bomb placed in deck and shuffled
	      Card newBomb = Card(0, "Cat Bomb");
	      m_deck.push_back(newBomb);
	      Shuffle();
	    }
	  }
	  
	  currentTurn = false;
	  break;
	}
	
      
     
      }

      // else if player has no cards they must only draw
      // will lose or diffuse if bomb is drawn
      else{

	//draw
	int bomb = m_players[i].AddToHand(DrawCard());
        if(bomb == 0){
          cout << "You have drawn a bomb!!!" << endl;
          Card newCard = m_players[i].ReturnBomb();

          if(newCard.GetType() == 0){
            m_players[i].SetLost(true);
            //adds one to lostPlayers
            lostPlayers ++;
            cout << "Player " << m_players[i].GetName() << " has lost!" << endl;
          }

          else{
	    //puts bomb back in deck if diffused
            m_deck.push_back(newCard);
            Shuffle();
          }
        }

        currentTurn = false;
      }
      
    }
    

    //checks if current player has an extra turn or not before going to next players turn
    if(m_players[i].HasExtraTurn() == true){
      cout << "Player " << m_players[i].GetName() << " gets another turn!" << endl; 
      m_players[i].SetExtraTurn(false);
    }

    else{
      //this if-else will iterate through turns in order
      if(i < m_numPlayers){
	i++;
      }
      else{
	i = 0;
      }
    }


  }//end of while(lostPlayers < gameOver) loop


  
}

int Game::LoadDeck(string name){

  int type = 0;
  string desc;
  string line;

  ifstream myfile;
  myfile.open("deck.txt");

  if(myfile.is_open()){
    
    while(getline(myfile,line)){

      //erases any spaces in order to convert to int
      line.erase(line.find_last_not_of(" \n\r\t")+1);
      type = atoi(line.c_str());
      desc = "";
    
      //adds effect of cards if they have one at the starts of descriptions before ojects are created
      if(type == 1 or type == 2 or type == 3 or type == 4 or type == 5){
	switch(type){
	case 1:
	  desc = "Defuse: ";
	  break;
	case 2:
	  desc = "Attack: ";
	  break;
	case 3:
	  desc = "Skip: ";
	  break;
	case 4:
	  desc = "Peek: ";
	  break;
	case 5:
	  desc = "Shuffle: ";
	  break;
	}
      }

      getline(myfile,line);
      line.erase(line.find_last_not_of(" \n\r\t")+1);

      desc.append(line);
      // creates object and then pushes to the back of deck vector
      Card fill = Card(type, desc);
      m_deck.push_back(fill);
    }
    myfile.close();
  }
  return 0;
}

int Game::AddBombCards(string name){

  int type = 0;
  string desc;
  string line;

  ifstream myfile;
  myfile.open("bomb.txt");

  //basically same code as the loaddeck() except no adding effects since these are only bombs
  if(myfile.is_open()){

    while(getline(myfile,line)){
      //erases spaces to convert to int
      line.erase(line.find_last_not_of(" \n\r\t")+1);
      type = atoi(line.c_str());
      getline(myfile,line);
      line.erase(line.find_last_not_of(" \n\r\t")+1);
      desc = line;

      Card fill = Card(type, desc);
      m_deck.push_back(fill);
    }
    myfile.close();
  }
  return 0;
}
