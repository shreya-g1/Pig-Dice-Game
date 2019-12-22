//pig.cpp
//Shreya Gowda, CISP 400
//9/8/19

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

//integer corresponding to each player
typedef enum {COMPUTER, HUMAN, NUM_PLAYERS} player_type;

/*struct is used to hold info about each game (number of games, winner, number of turns, etc.). 
each game has a pointer to the next game, making a linked list of games*/
struct GameInfo{
  int game_id=0;
  string winner="";
  string loser="";
  int turns=0;
  GameInfo* next;

  //constructor
  GameInfo(int n, int t, string w, string l){
    game_id=n;
    turns=t;
    winner=w;
    loser=l;
  }
};

// Function Prototype
string player_menu();
void roll(int* turn_points, player_type* whose_turn, string* option);
void hold(player_type* whose_turn, int* turn_points, int* grand);
void game_end_check(int* grand, string* option, const int TOTAL, string* winner, string* loser, string* name);
void resign(int* turn_points, player_type* whose_turn, int* turns_taken, int* grand);
void scoreboard(string* name, int* grand);
void ending_display(GameInfo* first_node);

int main() {
  const int MAX_TOTAL=100;           //the number of grand points before a player wins
  const string DEFAULT_WL="RESIGNED";   //the default string for winner and loser, in case the player resigns and there is no winner or loser
  player_type player_turn=COMPUTER;  //keeps track of whose turn it is
  string player_name[NUM_PLAYERS]={"Computer",""};    //array of the player's names
  string opt;         //option active player choses during their turn from the menu
  int turn_total=0;   //turn total during each player's turn
  int grand_total[NUM_PLAYERS]={0,0};   //grand points of each player during current game
  time_t t=time(0);   //variable containing time seed
  int num_games=0;    //how many games were played throughout program
  int num_turns=0;    //how many turns current game took
  string winner=DEFAULT_WL;   //the winner of current game
  string loser=DEFAULT_WL;    //the loser of current game
  GameInfo* first=nullptr;    //the first game's game information
  GameInfo* head=nullptr;     //the current game's game information
  GameInfo* next=nullptr;     //the following game's game information

  // Specification C1 - Time Seed
  srand(t);

  // Program Greeting
  // Specification A3 - Current Date
  cout<<ctime(&t);
  cout<<"Welcome to Pig Dice Game! You will be racing against the Computer to reach 100 grand points and win."<<endl;
  cout<<endl<<"Game Overview"<<endl;
  cout<<"Description\nYou gain points when you ROLL a die. You can roll the die multiple times on your turn. Each time you roll, your die number gets added to your temporary turn total. However, if you roll a 1, you lose all your turn total points and it's the other player's turn. If you want to keep your points, you must choose to HOLD-- this adds your turn total points to your permanent grand total, resets your turn total, and forfeits your turn to the other player. The player who reaches 100 grand total points first, wins. You can RESIGN (to start a new game) or QUIT (to end the program) anytime while it is your turn and the program is running."<<endl;
  cout<<"Controls\nWhen it is your turn, you can type the letter, enclosed in <brackets> corresponding to the option listed in the options menu. Your options are:\n  <R> for Roll\n  <H> for Hold\n  <S> for Resign\n  <Q> for Quit "<<endl;

  cout<<endl<<"Press enter to continue";
  cin.get();
  system("clear");

  // Specification C2 - Student Name
  cout<<endl<<"What is your name player? \nInput first and last name, then press enter."<<endl;
  getline(cin, player_name[HUMAN]);
  cout<<endl;

  //Specification B2 - Randomize Start
  player_turn=player_type(rand()%NUM_PLAYERS);
	
  

  // Specification A1 - Main Event Loop
  while(opt!="q"){

    //when player changes, display's whose turn it is and the grand scores
    scoreboard(player_name, grand_total);
    if(player_turn==COMPUTER){
      cout<<"Computer's turn"<<endl;
      opt="r";
    }
    else{
      cout<<player_name[player_turn]<<"'s turn"<<endl;
      opt=player_menu();
    }

    while(opt=="r"){ 
      //number of turns increases everytime players roll
      num_turns++;

      //for prettier display output. shows that it is human player's turn. also, pauses so player can see output of roll if roll is 1
      if(player_turn==HUMAN){
        cout<<endl<<"Press enter to continue";
  	    cin.get();
        scoreboard(player_name, grand_total);
        cout<<player_name[player_turn]<<"'s turn"<<endl;
      }

      roll(&turn_total, &player_turn, &opt);
    }
    if(opt=="h"){ 
      hold(&player_turn, &turn_total, grand_total);
      game_end_check(grand_total, &opt, MAX_TOTAL, &winner, &loser, player_name);
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
      resign(&turn_total, &player_turn, &num_turns, grand_total);
      winner=DEFAULT_WL;
      loser=DEFAULT_WL;
    }
  }

  //number of games after last game
  num_games++;

  //finds winner and loser of last game
  winner=DEFAULT_WL;
  loser=DEFAULT_WL;
  if(grand_total[COMPUTER]>=MAX_TOTAL){
    winner=player_name[COMPUTER];
    loser=player_name[HUMAN];
  }
  if(grand_total[HUMAN]>=MAX_TOTAL){
    winner=player_name[HUMAN];
    loser=player_name[COMPUTER];
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
  cout<<endl<<"Thanks for playing pig!"<<endl;
}

string player_menu(){
  /*shows display of menu. 
  gets and returns the option the player chose from the menu*/

  string choice;//the choice the player choses from the following menu

  // Specification C3 - REPLACED
  // Specification B3 - Alpha Menu
  cout<<"Menu Options"<<endl;
  cout<<"1. <R>oll"<<endl;
  cout<<"2. <H>old"<<endl;
  cout<<"3. Re<s>ign"<<endl;
  cout<<"4. <Q>uit"<<endl;

  cout<<endl<<"enter: ";
  cin>>choice;

  // Specification C4 - Bulletproof Menu
  // Specification A4 - Upper and Lower Case
  for(unsigned int i=0; i<choice.length(); i++){
    choice[i]=tolower(choice[i]);
  }
  while(choice!="r"&&choice!="h"&&choice!="s"&&choice!="q"){
    cout<<"Invalid input. Please retype: ";
    cin>>choice;
    for(unsigned int i=0; i<choice.length(); i++){
      choice[i]=tolower(choice[i]);
    }
  }

  return choice;
}

// Specification B1 - Track each turn
void roll(int* turn_points, player_type* whose_turn, string* option){
  /*when player rolls the die, gets the number that was rolled.
  decides if player goes again, holds, or loses their turn depending on number*/

  int dice=0;   //number the player rolled w/ die

  //player rolls die
  dice=rand()%6+1;

  if(dice==1){
    *turn_points=0;
    cout<<"dice: "<<dice<<"   turn total: "<<*turn_points<<endl<<endl;
    *option="h";
  }
  else{
    *turn_points+=dice;
    cout<<"dice: "<<dice<<"   turn total: "<<*turn_points<<endl<<endl;
    if(*whose_turn==HUMAN){
      *option=player_menu();
    }
    if(*whose_turn==COMPUTER){
      if(dice>=4){*option="r";}
      else{*option="h";}
    }
  }
}

void hold(player_type* whose_turn, int* turn_points, int* grand){
  /*adds turn total points to grand total. resets turn total.
  then switches players*/

  for(int i=0; i<NUM_PLAYERS; i++){
    if(*whose_turn==player_type(i)){
      grand[i]+=(*turn_points);
    }
  }
  *turn_points=0;

  //pauses before player changes to see computer's turn total
  cout<<"Press enter to continue";
  cin.get();
  system("clear");
  

  if(*whose_turn==HUMAN){*whose_turn=COMPUTER;}
  else{*whose_turn=HUMAN;}
}

void game_end_check(int* grand, string* option, const int MAX, string* winner, string* loser, string* name){
  /*checks if any player reached 100 grand points after they did "hold"*/

  string cont;  //continue to play pig (new game) or end program

  //if a player won, human can make new game or quit
  for(int i=0; i<NUM_PLAYERS; i++){
    if(grand[i]>=MAX){
      cout<<"Game End."<<endl;

      cout<<"Would you like to play again?(Y/N)";
      cin>>cont;
      cont[0]=tolower(cont[0]);
      while(cont!="y"&&cont!="n"){
        cout<<"Invalid input. Please retype: ";
        cin>>cont;
        cont[0]=tolower(cont[0]);
      }
      if(cont=="y"){*option="s";}
      else{*option="q";}
    }
  }

  //if someone won, they are stored as winner for game and the other play is loser 
  if(grand[COMPUTER]>=MAX){
    *winner=name[COMPUTER];
    *loser=name[HUMAN];
  }
  if(grand[HUMAN]>=MAX){
    *winner=name[HUMAN];
    *loser=name[COMPUTER];
  }
}

void resign(int* turn_points, player_type* whose_turn, int* turns_taken,int* grand){
  /*makes new game by resetting grand scores, turn total, and # of turns and randomly chosing whose turn it is*/
  for(int i=0; i<NUM_PLAYERS; i++){
    grand[i]=0;
  }
  *turn_points=0;
  *turns_taken=0;

  *whose_turn=player_type(rand()%NUM_PLAYERS);
}

void scoreboard(string* name, int* grand){
  /*prints ascii art and score*/
  system("clear");
  cout<<"      (O)          ############# "<<endl;
  cout<<" ______|______     ##         ## "<<endl;
  cout<<"|             |    #  ==   ==  # "<<endl;
  cout<<"|  [ ]   [ ]  |    # (O)   (O) # "<<endl;
  cout<<"|             |   (     |_      )"<<endl;
  cout<<"|    <III>    |    |    ___    | "<<endl;
  cout<<"|_____________|     |_________|  "<<endl<<endl;
  for(int i=0; i<NUM_PLAYERS; i++){
    cout<<name[i]<<": "<<grand[i]<<"         ";
  }
  cout<<endl<<endl;
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


