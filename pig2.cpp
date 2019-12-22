//pig2.cpp
//Shreya Gowda, CISP 400
//9/8/19

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

//integer corresponding to each player
typedef enum {PLAYER2, PLAYER1, NUM_PLAYERS} player_type;
typedef enum {COMPUTER, HUMAN} player_mode_type;

/*struct is used to hold info about each game (number of games, winner, number of turns, etc.). 
each game has a pointer to the next game, making a linked list of games*/
struct GameInfo{
  int game_id;
  string winner;
  string loser;
  int turns;
  GameInfo* next;

  //constructor
  GameInfo(int n, int t, string w, string l){
    game_id=n;
    turns=t;
    winner=w;
    loser=l;
  }
};

class Player{
  private:
  int turn_total;
  int grand_total;
  string player_name;

  public:
  Player(){
    turn_total=0;
    grand_total=0;
    player_name="";
  }

  void setName(string n){
    player_name=n;
  }

  string getName(){
    return player_name;
  }

  // Specification B1 - Track each turn
  string roll(player_mode_type player){
    /*when player rolls the die, gets the number that was rolled.
    decides if player goes again, holds, or loses their turn depending on number*/

    int dice=0;   //number the player rolled w/ die

    //player rolls die
    dice=rand()%6+1;

    if(dice==1){
      turn_total=0;
      cout<<"dice: "<<dice<<"   turn total: "<<turn_total<<endl<<endl;
      return "h";
    }
    else{
      turn_total+=dice;
      cout<<"dice: "<<dice<<"   turn total: "<<turn_total<<endl<<endl;
        if(player==HUMAN){
          return "player menu";
      }
      if(player==COMPUTER){
        if(dice>=4){return "r";}
        else{return "h";}
      }
    }

    return "wtf";
  }

  void hold(){
    /*adds turn total points to grand total. resets turn total.
    then switches players*/

    grand_total+=turn_total;
    turn_total=0;

    //pauses before player changes to see computer's turn total
    cout<<"Press enter to continue";
    cin.get();
    system("clear");
  }

  void reset(){
    /*resets grand_total and turn_total*/
    grand_total=0;
    turn_total=0;
  }

  int getGrand(){
    return grand_total;
  }
};

void instructions();
string player_menu();
void scoreboard(Player player1, Player player2);
void game_end_check(Player player1, Player player2, string* opt, string* winner, string* loser);
void ending_display(GameInfo* first_node);
string validInput(const string* valid, const int arr_size);

int main(){
  const int MAX_TOTAL=100;           //the number of grand points before a player wins
  const string DEFAULT_WL="RESIGNED";   //the default string for winner and loser, in case the player resigns and there is no winner or loser
  const int NUM_MODES=2;
  const string VALID_GAME_MODES[NUM_MODES]={"pvp", "comp"};
  string gamemode="";  //Player vs Player or Player vs Computer
  Player player2; 
  Player player1;   
  player_type player_turn=PLAYER2;  //keeps track of whose turn it is
  string opt;         //option active player choses during their turn from the menu
  time_t t=time(0);   //variable containing time seed
  int num_games=0;    //how many games were played throughout program
  int num_turns=0;    //how many turns current game took
  string winner=DEFAULT_WL;   //the winner of current game
  string loser=DEFAULT_WL;    //the loser of current game
  GameInfo* first=nullptr;    //the first game's information
  GameInfo* head=nullptr;     //the current game's information
  GameInfo* next=nullptr;     //the following game's information

  // Specification C1 - Time Seed
  srand(t);

  // Program Greeting
  // Specification A3 - Current Date
  cout<<ctime(&t);
  cout<<"Welcome to Pig Dice Game! You will be racing against the Computer or another player to reach 100 grand points and win."<<endl;
  instructions();

  cout<<endl<<"Press enter to continue";
  cin.get();
  system("clear");

  cout<<"Would you like to play against another player (PVP) or against the computer (COMP)? \nInput game mode, then press enter."<<endl;
  gamemode=validInput(VALID_GAME_MODES, NUM_MODES);

  // Specification C2 - Student Name
  cout<<endl<<"What is your name player one? \nInput first and last name, then press enter."<<endl;
  getline(cin,opt);
  player1.setName(opt);

  if(gamemode=="pvp"){
    cout<<endl<<"What is your name player two? \nInput first and last name, then press enter."<<endl;
    getline(cin, opt);
    player2.setName(opt);
  }
  else{
    player2.setName("Computer");
  }
  cout<<endl;

  //Specification B2 - Randomize Start
  player_turn=player_type(rand()%NUM_PLAYERS);
	
  // Specification A1 - Main Event Loop
  while(opt!="q"){

    //when player changes, display's whose turn it is and the grand scores
    scoreboard(player2, player1);
    if(player_turn==PLAYER2){
      cout<<player2.getName()<<"'s turn"<<endl;
      if(player2.getName()=="Computer"){
        opt="r";
      }
      else{
        opt=player_menu();
      }
    }
    else{
      cout<<player1.getName()<<"'s turn"<<endl;
      opt=player_menu();
    }

    while(opt=="r"){ 
      //number of turns increases everytime players roll
      num_turns++;

      //for prettier display output. shows that it is human player's turn. also, pauses so player can see output of roll if roll is 1
      if(player_turn==PLAYER1){
        cout<<endl<<"Press enter to continue";
  	    cin.get();
        scoreboard(player2, player1);
        cout<<player1.getName()<<"'s turn"<<endl;
        opt=player1.roll(HUMAN);
        if(opt=="player menu"){
          opt=player_menu();
        }
      }
      else{
        if(gamemode=="pvp"){
          cout<<endl<<"Press enter to continue";
  	      cin.get();
          scoreboard(player2, player1);
          cout<<player2.getName()<<"'s turn"<<endl;
          opt=player2.roll(HUMAN);
          if(opt=="player menu"){
            opt=player_menu();
          }
        }
        else{
          opt=player2.roll(COMPUTER);
        }
      }   
    }
    if(opt=="h"){ 
      if(player_turn==PLAYER1){
        player1.hold();
        player_turn=PLAYER2;
      }
      else{
        player2.hold();
        player_turn=PLAYER1;
      }
      game_end_check(player2, player1, &opt, &winner, &loser);
    }
    // Specification B4 – Resign Option
    if(opt=="s"){ 
      //number of games increases everytime new game is formed
      num_games++;

      // Specification A2 - Group Stats
      if(first==NULL){
        head=new GameInfo(num_games, num_turns, winner, loser);
        first=head;
      }
      else{
        next=new GameInfo(num_games, num_turns, winner, loser);
        head->next=next;
        head=next;
      }

      //resets all relevant variables to start new game
      player2.reset();
      player1.reset();
      num_turns=0;
      player_turn=player_type(rand()%NUM_PLAYERS);
      winner=DEFAULT_WL;
      loser=DEFAULT_WL;
    }
  }

  //number of games after last game
  num_games++;

  //finds winner and loser of last game
  winner=DEFAULT_WL;
  loser=DEFAULT_WL;
  if(player2.getGrand()>=MAX_TOTAL){
    winner=player2.getName();
    loser=player1.getName();
  }
  if(player1.getGrand()>=MAX_TOTAL){
    winner=player1.getName();
    loser=player2.getName();
  }

  //inputs data for last game into last element of list
  if(first==nullptr){
    head=new GameInfo(num_games, num_turns, winner, loser);
    first=head;
  }
  else{
    next=new GameInfo(num_games, num_turns, winner, loser);
    head->next=next;
    head=next;
  }
  //sets the next game after the last game to NULL because it doesn't exist
  next=nullptr;
  head->next=next;

  ending_display(first);
  cout<<endl<<"Thanks for playing Pig!"<<endl;

}

void instructions(){
  cout<<endl<<"Game Overview"<<endl;
  cout<<"Description\nYou gain points when you ROLL a die. You can roll the die multiple times on your turn. Each time you roll, your die number gets added to your temporary turn total. However, if you roll a 1, you lose all your turn total points and it's the other player's turn. If you want to keep your points, you must choose to HOLD-- this adds your turn total points to your permanent grand total, resets your turn total, and forfeits your turn to the other player. The player who reaches 100 grand total points first, wins. You can RESIGN (to start a new game) or QUIT (to end the program) anytime while it is your turn and the program is running."<<endl;
  cout<<"Controls\nWhen it is your turn, you can type the letter, enclosed in <brackets> corresponding to the option listed in the options menu. Your options are:\n  <R> for Roll\n  <H> for Hold\n  <S> for Resign\n  <Q> for Quit "<<endl;
}

string player_menu(){
  /*shows display of menu. 
  gets and returns the option the player chose from the menu*/
  const int ARR_SIZE=4;
  const string VALID_OPTS[ARR_SIZE]={"r","h","s","q"};
  string choice;//the choice the player choses from the following menu

  // Specification C3 - REPLACED
  // Specification B3 - Alpha Menu
  cout<<"Menu Options"<<endl;
  cout<<"1. <R>oll"<<endl;
  cout<<"2. <H>old"<<endl;
  cout<<"3. Re<s>ign"<<endl;
  cout<<"4. <Q>uit"<<endl<<endl;

  // Specification C4 - Bulletproof Menu
  choice=validInput(VALID_OPTS, ARR_SIZE);

  return choice;
}

void game_end_check(Player player1, Player player2, string* opt, string* winner, string* loser){
  /*checks if any player reached 100 grand points after they did "hold"*/
  const int MAX=100;
  const int ARR_SIZE=2;
  const string VALID_INPUT[ARR_SIZE]={"y","n"};

  string cont;  //continue to play pig (new game) or end program

  //if someone won, they are stored as winner for game and the other play is loser 
  if(player1.getGrand()>=MAX){
    *winner=player1.getName();
    *loser=player2.getName();
  }
  if(player2.getGrand()>=MAX){
    *winner=player2.getName();
    *loser=player1.getName();
  }

  if(player1.getGrand()>=MAX||player2.getGrand()>=MAX){
    cout<<"Game End. Winner is "<<*winner<<endl;

    cout<<"Would you like to play again?(Y/N)"<<endl;
    cont=validInput(VALID_INPUT, ARR_SIZE);
    if(cont=="y"){*opt= "s";}
    else{*opt= "q";}
  }
}

void ending_display(GameInfo* first_node){
  /*prints every game's information in a table at the end of the program*/

  GameInfo* game=nullptr;   //holds game info as it loops through all the games
  game=first_node;
  system("clear");
  cout<<"Games Info"<<endl;
  cout<<"#     winner         loser         turns"<<endl;
  //loops through all games and prints info until there are no more (null)
  while(game!=nullptr){
    cout<<left<<setw(6)<<game->game_id<<setw(15)<<game->winner<<setw(15)<<game->loser;
    cout<<right<<setw(4)<<game->turns<<endl;
    game=game->next;
  }
}

void scoreboard(Player player1, Player player2){
  /*prints ascii art and score*/
  system("clear");
  cout<<"      (O)          ############# "<<endl;
  cout<<" ______|______     ##         ## "<<endl;
  cout<<"|             |    #  ==   ==  # "<<endl;
  cout<<"|  [ ]   [ ]  |    # (O)   (O) # "<<endl;
  cout<<"|             |   (     |_      )"<<endl;
  cout<<"|    <III>    |    |    ___    | "<<endl;
  cout<<"|_____________|     |_________|  "<<endl<<endl;
  cout<<player1.getName()<<": "<<player1.getGrand()<<"         ";
  cout<<player2.getName()<<": "<<player2.getGrand();
  cout<<endl<<endl;
}

string validInput(const string* valid, const int arr_size){
  string input="";
  bool bad_input;

  do{
    bad_input=true;
    cout<<"enter: ";
    getline(cin, input);
    // Specification A4 - Upper and Lower Case
    for(unsigned int i=0; i<input.length(); i++){
      input[i]=tolower(input[i]);
    }
    for(int i=0;i<arr_size; i++){
      if(valid[i]==input){
        bad_input=false;
      }
    }
    if(bad_input){
      cout<<"Invalid input, please retype."<<endl;
    }
  }while(bad_input);

  return input;
}
