/*  The following code builds a Simon Says Game using an Arduino microcontroller.
    The user has to repeat a given pattern shown by LEDs by pressing buttons.
    The game is lost when said pattern cannot be repeated in the right way or 
    no button has been pressed for three seconds.

    Made by Jule Voskamp and Jacob Tumak
    Last modified on Dec. 04th, 2022  */

#include <time.h>

const int LEFT_LIGHT = 13;
const int RIGHT_LIGHT = 12;
const int SPEAKER = 8;
const int LEFT_BUTTON = 4;
const int RIGHT_BUTTON = 2;
const int LEFT = 1;
const int RIGHT = 2;

/*––––––––––––Prototypes––––––––––––*/
void PlayGame(void);
int PlayFollower(int, int[]);
void GenLeader(int[], int);
void PlaySpeakerStart();
void PlaySpeakerLost();
void NextRoundSpeaker();
/*––––––––––––––––––––––––––––––––––*/

/*–––––––––––––––SETUP––––––––––––––*/
void setup() {
  pinMode(LEFT_LIGHT, OUTPUT);  
  pinMode(RIGHT_LIGHT, OUTPUT); 
  pinMode(SPEAKER, OUTPUT); 
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  srand((unsigned) time(NULL));
}
/*––––––––––––––––––––––––––––––––––*/

/*––––––––––––MAIN–LOOP–––––––––––––*/
void loop() {
  PlayGame();
  delay(1000);
}
/*––––––––––––––––––––––––––––––––––*/

/*––––––MAIN–PLAYGAME–FUNCTION––––––*/
void PlayGame(void){
  int leader[20];
  int checkStatus = 1;
  static int round = 1;

  PlaySpeakerStart();
  delay (1500);
  do{
    PlayLeader(leader, round);    
    checkStatus = PlayFollower(round, leader);
    if (checkStatus == -1)
    round = 0;
    
    ++round;
    delay(1500);
    
    }while(checkStatus != -1 && round < 20);

}
/*––––––––––––––––––––––––––––––––––*/

/*–––––––––––USERTURN–LOGIC–––––––––*/
int PlayFollower(int round, int leader[]){
  int LBtn = LOW, RBtn = LOW;               //initialize both buttons with "LOW" state
  int i = 0;
  unsigned long timeStart = millis();
  int timeCheck;

  do{ 
  RBtn = digitalRead(RIGHT_BUTTON); LBtn = digitalRead(LEFT_BUTTON); //Get Button State

  timeCheck = millis() - timeStart;
  if(timeCheck > 3000){   //more than 3 seconds passed since pressed a button
  PlaySpeakerLost();
  return -1;}

  if(RBtn == HIGH){   //right button is pressed
    while(RBtn == HIGH){
    RBtn = digitalRead(RIGHT_BUTTON);
    digitalWrite(RIGHT_LIGHT, HIGH);}

    if(leader[i] == RIGHT){ //pressed button fits LED that blinked
      timeStart = millis(); //Resets time since last click
      ++i;
    }
      else{                 //pressed button does not fit LED that blinked
         digitalWrite(RIGHT_LIGHT, LOW);
        PlaySpeakerLost();
      return -1;}
      delay(100);
      digitalWrite(RIGHT_LIGHT, LOW);  
  }

  else if(LBtn == HIGH){ //left button is pressed
    while(LBtn == HIGH){
    LBtn = digitalRead(LEFT_BUTTON);
    digitalWrite(LEFT_LIGHT, HIGH);}

    if(leader[i] == LEFT){  //pressed button fits LED that blinked
      timeStart = millis(); // Resets time since last click
    ++i;
    }
    else{                   //pressed button does not fit LED that blinked
       digitalWrite(LEFT_LIGHT, LOW);
      PlaySpeakerLost();
    return -1;}
      delay(100);
      digitalWrite(LEFT_LIGHT, LOW);
  }

  }while(i < round);
  NextRoundSpeaker();
  return 1;
}
/*––––––––––––––––––––––––––––––––––*/

/*––––––CREATES–RANDOM–ARRAY––––––––*/
void GenLeader(int leader[], int round)
{
  int i;
  
  for (i = 0; i < round; ++i)
  {
    leader[i] = 1 + (rand() % 2);
  }
}
/*––––––––––––––––––––––––––––––––––*/

/*––––––––––––PLAYS–SIMON–––––––––––*/
void PlayLeader(int leader[], int round){
  int i;
  
  GenLeader(leader, round);
  
  for(i = 0; i < round; ++i){
    if(leader[i] == LEFT){
      digitalWrite(LEFT_LIGHT, HIGH);
      delay(250);
      digitalWrite(LEFT_LIGHT, LOW);
    }

    else if (leader[i] == RIGHT){
      digitalWrite(RIGHT_LIGHT, HIGH);
      delay(250);
      digitalWrite(RIGHT_LIGHT, LOW);
    }
      delay(200);
  }
}
/*––––––––––––––––––––––––––––––––––*/

/*––––––––TONE–FOR–GAME–START–––––––*/
void PlaySpeakerStart() 
{
  int i;
  
  for(i=1000; i < 1400; i = i + 100) {
    tone(8, i, 50);
    delay(250);
  }
}
/*––––––––––––––––––––––––––––––––––*/

/*––––––––TONE–FOR–NEXT–ROUND–––––––*/
void NextRoundSpeaker()
{ 
   int i;
  
  for(i=1000; i < 1400; i = i + 100) {
    tone(8, i, 50);
    delay(50);
  }
}
/*––––––––––––––––––––––––––––––––––*/

/*–––––––––TONE–IF–GAME–LOST––––––––*/
void PlaySpeakerLost()
{
    tone(8, 600, 150);
  	delay(500);
    tone(8, 600, 300);
    delay(500);
    tone(8, 600, 450);
  
} 
/*––––––––––––––––––––––––––––––––––*/
