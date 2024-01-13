//#include <iostream>
//#include <cstdlib>
//#include <conio.h>
//#include <ctime>
using namespace std;
#include <FastLED.h>
#define NUM_LEDS 64
#define DATA_PIN 5
#define CLOCK_PIN 13
CRGB leds[NUM_LEDS];
//translates LED pins into array
int cds[42] = {14, 17, 30, 33, 46, 49, 62, 13, 18, 29, 34, 45, 50, 61, 12, 19, 28, 35, 44, 51, 60, 11, 20, 27, 36, 43, 52, 59, 10, 21, 26, 37, 42, 53, 58, 9, 22, 25, 38, 41, 54, 57};
int colcds[7] = {56,55,40,39,24,23,8};
bool provocation = false; // used to display a provocative screen
char input[43]; // There are 42 places to play in the board .. this array represent them
int PlayOut = 0;
int EVA = 0;
int totaldepth = 1;
// EVA and PlayOut are working while NegaMax Function is working .. in NegaMax's way it will definitely make any winning move
// as it goes deep so when this happens PlayOut increases by 1 and EVA increases by 1 if its a winnning move or decreases by 1 if it the opposite
// so we can evaluate neutral moves for MiniMax by dividing EVA / PlayOut * 100 so we get a percentage
int winning();
int GetValue(int t);
int AIManager();
int NegaMax(int Depth);
void clean();
int GetValue(int column);
void clear();
void Board();
void PlayPosition(char XO);
int animatecolumn;
int animaterow;
int trail0[6] = {57,58,59,60,61,62};
int trail1[6] = {54,53,52,51,50,49};
int trail2[6] = {41,42,43,44,45,46};
int trail3[6] = {38,37,36,35,34,33};
int trail4[6] = {25,26,27,28,29,30};
int trail5[6] = {22,21,20,19,18,17};
int trail6[6] = {9,10,11,12,13,14};
int winningcoords[4];
int lastAImove;

void setup() {
  //LED Business 
  LEDS.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(80);
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  screensaver();
  clean();
}

void loop() // main function
{
    srand(analogRead(A1));
    clean();
    int winningtemp = 0;
    do
    {
        input[AIManager()]='O';
        delay(200);
        Board(1);
        outputint();
        winningtemp = winning();
        if(winningtemp!=0)
        {
            if(winningtemp == 1){

                Serial.println("Player 2 won!");
                Serial.print(winningcoords[0]);
                Serial.print(", ");
                Serial.print(winningcoords[1]);
                Serial.print(", ");
                Serial.print(winningcoords[2]);
                Serial.print(", ");
                Serial.print(winningcoords[3]);
                Serial.println(" ");

                flashwin(2);
                delay(1000);
                break;
            }else if (winningtemp == 2){
                Serial.println("Player 1 won!");

                Serial.print(winningcoords[0]);
                Serial.print(", ");
                Serial.print(winningcoords[1]);
                Serial.print(", ");
                Serial.print(winningcoords[2]);
                Serial.print(", ");
                Serial.print(winningcoords[3]);
                Serial.println(" ");

                flashwin(1);
                delay(1000);
                break;
            }else if (winningtemp == 3){}
            Serial.println("Tie");
             break;
        } else {
            PlayPosition('X');
            winningtemp = winning();
            animate();
            Board(0);
        }
        if(winningtemp!=0)
        {
            if(winningtemp == 1){
                Serial.println("Player 2 won!");

                Serial.print(winningcoords[0]);
                Serial.print(", ");
                Serial.print(winningcoords[1]);
                Serial.print(", ");
                Serial.print(winningcoords[2]);
                Serial.print(", ");
                Serial.print(winningcoords[3]);
                Serial.println(" ");

                
                flashwin(2);
                
                break;
                delay(1000);
            }else if (winningtemp == 2){
                Serial.println("Player 1 won!");

                Serial.print(winningcoords[0]);
                Serial.print(", ");
                Serial.print(winningcoords[1]);
                Serial.print(", ");
                Serial.print(winningcoords[2]);
                Serial.print(", ");
                Serial.print(winningcoords[3]);
                Serial.println(" ");


                flashwin(1);
                delay(1000);
                break;
            }else if (winningtemp == 3){}
            Serial.println("Tie");
             break;
        }
    } while(true);

}

void flashwin(int player){
  for(int i = 0; i <=6; i++){
      leds[colcds[i]] = CRGB :: Black;
  }
  for(int i = 0; i <=3; i++){
      leds[cds[winningcoords[i]]] = CRGB :: Black;
  }
  FastLED.show();
  delay(100);
  if (player == 2){
      for(int i = 0; i <=3; i++){
      leds[cds[winningcoords[i]]] = CRGB :: Green;
      }
      FastLED.show();

      delay(300);
      for(int i = 0; i <=3; i++){
      leds[cds[winningcoords[i]]] = CRGB :: Black;
      }
      FastLED.show();
      delay(300);
      for(int i = 0; i <=3; i++){
          leds[cds[winningcoords[i]]] = CRGB :: Green;
      }
      FastLED.show();
      delay(300);
      for (int i=0; i<42; i++){
      leds[cds[i]] = CRGB :: Black;
      }
      for(int i = 0; i <=3; i++){
          leds[cds[winningcoords[i]]] = CRGB :: Green;
      }
      delay(400);
      FastLED.show();
      delay(600);

  } else {
      for(int i = 0; i <=3; i++){
      leds[cds[winningcoords[i]]] = CRGB :: Yellow;
      }
      FastLED.show();

      delay(300);
      for(int i = 0; i <=3; i++){
      leds[cds[winningcoords[i]]] = CRGB :: Black;
      }
      FastLED.show();
      delay(300);
      for(int i = 0; i <=3; i++){
      leds[cds[winningcoords[i]]] = CRGB :: Yellow;
      }
      FastLED.show();
      delay(300);
      for (int i=0; i<42; i++){
      leds[cds[i]] = CRGB :: Black;
      }
      for(int i = 0; i <=3; i++){
          leds[cds[winningcoords[i]]] = CRGB :: Yellow;
      }
      delay(400);
      FastLED.show();
      delay(600);
  }
}

void outputint(){
  Serial.println(" ");
  for (int j = 5; j>=0; j--){
    for (int i = 0; i<=7; i++){
    Serial.print(j*7 + i);  
    Serial.print(input[j*7 + i]);
    Serial.print(", ");
  }
    Serial.println(" ");
  }

}

void animate(){
  int trail[6-animaterow]; 
  if (animatecolumn == 0){
    for (int i = 0; i < (6-animaterow) ; i++){
    trail[i] = trail0[i];
  }
  }else if(animatecolumn == 1){
   for (int i = 0; i < (6-animaterow) ; i++){
    trail[i] = trail1[i];
  }
  }else if(animatecolumn == 2){
    for (int i = 0; i < (6-animaterow) ; i++){
    trail[i] = trail2[i];
  }
  }else if(animatecolumn == 3){
    for (int i = 0; i < (6-animaterow) ; i++){
    trail[i] = trail3[i];
  } 
  }else if(animatecolumn == 4){
    for (int i = 0; i < (6-animaterow) ; i++){
    trail[i] = trail4[i];
  } 
  }else if(animatecolumn == 5){
    for (int i = 0; i < (6-animaterow) ; i++){
    trail[i] = trail5[i];
  } 
  }else if(animatecolumn == 6){
    for (int i = 0; i < (6-animaterow) ; i++){
    trail[i] = trail6[i];
  }
  }
  for (int i = 0; i < (6-animaterow); i++) {
    leds[trail[i]] = CRGB :: Green;
    leds[0] = CRGB :: Black;
    delay(20);
    FastLED.show();
    delay(20);
    leds[trail[(i+1)]] = CRGB :: Green;
    leds[0] = CRGB :: Black;
    leds[8] = CRGB :: Black;
    delay(20);
    FastLED.show();
    leds[0] = CRGB :: Black;
    leds[8] = CRGB :: Black;
    FastLED.show();
  }
    for (int i = 0; i < (5-animaterow); i++) {
    leds[trail[i]] = CRGB :: Black;
    leds[0] = CRGB :: Black;
    leds[8] = CRGB :: Black;
    delay(10);
    leds[0] = CRGB :: Black;
    leds[8] = CRGB :: Black;
    delay(10);
    FastLED.show();
    leds[0] = CRGB :: Black;
    leds[8] = CRGB :: Black;
    FastLED.show();
  }

  leds[0] = CRGB :: Black;
  leds[8] = CRGB :: Black;
  FastLED.show();
}

void clean()
{
    provocation = false;
  for(int i = 0 ; i<= 42 ; i++){
        input[i]=' ';
  }
   for (int i=0; i<42; i++){
    leds[cds[i]] = CRGB :: Black;
  }
  FastLED.show();
}

int GetValue(int column) // pass this function a column that you want to play in and it will return its value in input array ..
{
    if(column > 7)
        return 0;
    int n;
    for(int i = 0 ; i<= 6 ; i++)
    {
        if(input[column+7*i] == ' ' )
        {
            n = column+7*i;
            animaterow = i;
            break;

    }

    }
    
    if ( n > 42 ){
      return 100;
    }
    return n;
}

void flash(int col){
  int flashcol[7] = {8,23,24,39,40,55,56};
  leds[flashcol[col]] = CRGB :: Black;
  FastLED.show();
  delay(100);
  leds[flashcol[col]] = CRGB :: Green;
  FastLED.show();
  delay(100);
  leds[flashcol[col]] = CRGB :: Black;
  FastLED.show();
  delay(100);
  leds[flashcol[col]] = CRGB :: Green;
  FastLED.show();
  delay(100);
    leds[flashcol[col]] = CRGB :: Black;
  FastLED.show();
  delay(100);
  leds[flashcol[col]] = CRGB :: Green;
  FastLED.show();
  delay(100);
}

int winning() // Winning algorithm
{
    int temp=0;
    for(int i = 0 ; i<= 42 ; i++)
    {
        if(input[i] != ' ')
        {
            temp++;
            if( i - int((i-1)/7) * 7 <= 4  ){
                if( input[i] == input [i+1] && input[i] == input[i+2] && input[i] == input[i+3] ){

                    winningcoords[0] = i;
                    winningcoords[1] = i+1;
                    winningcoords[2] = i+2;
                    winningcoords[3] = i+3;

                    if(input[i] == 'X' ){
                        return 1 ;
                    }else{
                        return 2;
                    }
                }
            }
            if( i <= 21 ){
                if ( input[i] == input[i+7] && input[i] == input[i+14] && input[i] == input[i+21]  ){
                    winningcoords[0] = i;
                    winningcoords[1] = i+7;
                    winningcoords[2] = i+14;
                    winningcoords[3] = i+21;

                    if(input[i] == 'X' ){
                        return 1 ;
                    }else{
                        return 2;
                    }
                }
            }
            if( i - int((i-1)/7) * 7 <= 4 && i<=18  ){
                if(input[i] == input[i+8] && input[i] == input[i+16] && input[i]==input[i+24]){
                    winningcoords[0] = i;
                    winningcoords[1] = i+8;
                    winningcoords[2] = i+16;
                    winningcoords[3] = i+24;

                    if(input[i] == 'X' ){
                        return 1 ;
                    }else{
                        return 2;
                    }
                }
            }
            if( i - int((i-1)/7) * 7 >= 4 && i <= 21   ){
                if(input[i] == input[i+6] && input[i] == input[i+12] && input[i]==input[i+18]){
                    winningcoords[0] = i;
                    winningcoords[1] = i+6;
                    winningcoords[2] = i+12;
                    winningcoords[3] = i+18;
                    if(input[i] == 'X' ){
                        return 1 ;
                    }else{
                        return 2;
                    }
                }
            }
        }

    }
    if ( input[0] == input[7] && input[0] == input[14] && input[0] == input[21]  ){
                    winningcoords[0] = 0;
                    winningcoords[1] = 7;
                    winningcoords[2] = 14;
                    winningcoords[3] = 21;
                    if(input[0] == 'X' ){
                        return 1 ;
                    }else if (input[0] == 'O'){
                        return 2;
                    }
    }
    if ( input[7] == input[14] && input[7] == input[21] && input[7] == input[28]  ){
                    winningcoords[0] = 7;
                    winningcoords[1] = 14;
                    winningcoords[2] = 21;
                    winningcoords[3] = 28;
                    if(input[7] == 'X' ){
                        return 1 ;
                    }else if (input[7] == 'O'){
                        return 2;
                    }
    }    
    if ( input[14] == input[21] && input[14] == input[28] && input[14] == input[35]  ){
                    winningcoords[0] = 14;
                    winningcoords[1] = 21;
                    winningcoords[2] = 28;
                    winningcoords[3] = 35;
                    if(input[14] == 'X' ){
                        return 1 ;
                    }else if (input[14] == 'O') {
                        return 2;
                    }
    }
    for (int i=0; i<6; i++){
      if ( input[i*7] == input[i*7+1] && input[i*7] == input[i*7+2] && input[i*7] == input[i*7+3]  ){
                    winningcoords[0] = i*7;
                    winningcoords[1] = i*7 +1;
                    winningcoords[2] = i*7 +2;
                    winningcoords[3] = i*7 +3;
                    if(input[i*7] == 'X' ){
                        return 1 ;
                    }else if (input[i*7] == 'O') {
                        return 2;
                    }
    }
    }

    if (input[0] == input[1] && input[0] == input[2] && input[0] == input[3]){
                        winningcoords[0] = 0;
                        winningcoords[1] = 1;
                        winningcoords[2] = 2;
                        winningcoords[3] = 3;
                        if(input[0] == 'X' ){
                            return 1 ;
                        }else if (input[1] == 'O') {
                            return 2;
                        }
    }

    for (int i=0; i<4; i++){ 
       if ( input[i*7] == input[i*7+8] && input[i*7] == input[i*7+16] && input[i*7] == input[i*7+24]  ){
                    winningcoords[0] = i*7;
                    winningcoords[1] = i*7 +8;
                    winningcoords[2] = i*7 +16;
                    winningcoords[3] = i*7 +24;
        if(input[i*7] == 'X' ){
                        return 1 ;
                    }else if (input[i*7] == 'O') {
                        return 2;
                    }
    }
    }
    for (int i=5; i>2; i--){ 
       if ( input[i*7] == input[i*7-6] && input[i*7] == input[i*7-12] && input[i*7] == input[i*7-18]  ){
                    winningcoords[0] = i*7;
                    winningcoords[1] = i*7 -6;
                    winningcoords[2] = i*7 -12;
                    winningcoords[3] = i*7 -18;
        if(input[i*7] == 'X' ){
                        return 1 ;
                    }else if (input[i*7] == 'O') {
                        return 2;
                    }
    }
    }           

    if (temp == 42){
        return 3;
    }    
    return 0;
}

void clear(){
  for (int i=0; i<42; i++){
    leds[cds[i]] = CRGB :: Black;
  }
  FastLED.show();
}

void Board(int AI) // Draw board
{
    //cout<<endl<<"    1   "<<"    2   "<<"    3   "<<"    4   "<<"    5   "<<"    6   "<<"    7   "<<endl;
  for(int i=0; i<42 ; i++){
    if (input[i] == 'X'){
      leds[cds[i]] = CRGB :: Green;
    } else if (input[i] == 'O') {
      leds[cds[i]] = CRGB :: Yellow;
    } else {
      leds[cds[i]] = CRGB :: Black;
    }
    FastLED.show();
  }
  if (AI == 1){
    leds[cds[lastAImove]] = CRGB :: Black;
    FastLED.show();
    delay(100);
    leds[cds[lastAImove]] = CRGB :: Yellow;
    FastLED.show();

    AI = 0;
  }
/*
    int j = 42;
    for(int i = 0 ; i<= 23 ; i++)
    {
        if(i % 4 == 0){
            //cout<<string(57,'-');
        }else
        {
            if( (i - 2) % 4 == 0)
            {
                j=42-(0.25*i+0.5)*6-((0.25*i+0.5)-1) ;
                for(int i = 0 ; i<=6 ; i++)
                {
                    //cout<<"|"<<"   "<<input[j]<<"   ";
                    j++;
                }
                //cout<<"|";
            }
            else
            {
                for(int i = 0 ; i<=6 ; i++){}
                    //cout<<"|"<<string(7,' ');
                //cout<<"|";
            }
        }
        //cout<<endl;
    }
    //cout<<string(57,'-');
    */
    if(provocation == true){}
        //cout<<endl<<"Hehe I'm sure of my winning :D "<<endl;
}
int coord;


void PlayPosition(char XO) // Function that asks you to enter where you like to play
{

    int button;
    int sth;
    int drop = false;
    int colum = 3;
    //cout<<endl<<"where would you like to play"<<endl;
    leds[colcds[colum]] = CRGB :: Green;
    FastLED.show();
    leds[7] = CRGB :: Black;
    FastLED.show();
    
    while(drop == false)
    {
      button = digitalRead(4);
      if (button == HIGH){
        if (colum <= 0){
          colum = 0;
        } else {
        colum = colum - 1;
        Serial.println(colum);
        }
      leds[colcds[colum]] = CRGB :: Green;
      leds[colcds[colum+1]] = CRGB :: Black;
      if (colum != 6){
      leds[8] = CRGB :: Black;
      }
      FastLED.show();
        do{
          delay(10);
          button = digitalRead(4);
        } while (button == HIGH);
      }

      button = digitalRead(3);
      if (button == HIGH){
        if(input[41-colum] != ' '){
            flash(6-colum);
            do{
              delay(10);
              button = digitalRead(3);
            } while (button == HIGH);
        } else {
          drop = true;
          Serial.println("drop");
            do{
              delay(10);
              button = digitalRead(3);
            } while (button == HIGH);
            leds[colcds[colum]] = CRGB :: Black;
        }

        }
      

      button = digitalRead(2);
      if (button == HIGH){
        if (colum >= 6){
          colum = 6;
        }else {
        colum = colum + 1;
        Serial.println(colum);
        }
      leds[colcds[colum]] = CRGB :: Green;
      leds[colcds[colum-1]] = CRGB :: Black;
      if (colum != 6){
      leds[8] = CRGB :: Black;
      }
      FastLED.show();
      do{
          delay(10);
          button = digitalRead(2);
        } while (button == HIGH);
      }


    FastLED.show();
    }
    animatecolumn=(colum);
    sth=GetValue(6-colum);
    if (sth < 100){
    Serial.print("column input is ");
    Serial.println(sth);
    delay(100);
    input[sth] = XO;
    }
}


int AIManager() // AI Algorithm
{
    float chance[2] = {9999999 , 0 };
    for(int column = 1 ; column <= 7 ; column ++)
    {
        PlayOut = 0;
        EVA=0;
        int PlayNumber = GetValue(column);
        if( PlayNumber != 0 && PlayNumber != 100 )
        {

            input[PlayNumber] = 'O';
            if(winning()==2)
               {
                   input[PlayNumber]=' ';
                   lastAImove = PlayNumber;
                   return PlayNumber ;
               } 
            float temp = -(100*NegaMax(1));
            if(PlayOut != 0)
                temp -= ((100*EVA)/PlayOut);
            if(-temp >= 100)
                provocation = true;
            if( chance[0] > temp  )
            {
                chance[0] = temp  ;
                chance[1] = PlayNumber;
            }
             //  cout<<endl<<-temp<<"   "<<EVA << "   " <<PlayOut;
            input[PlayNumber] = ' ';
        } 
    }
    lastAImove = chance[1];
    return chance[1];
}


int NegaMax(int Depth) // MiniMax algorithm in NegaMax form
{
    char XO;
    int PlayNumber[8] = {0,0,0,0,0,0,0,0}; // The values of the input[] for every column
    int chance=0;
    if(Depth % 2 != 0){
        XO='X';
    }else{
        XO='O';
    }  
    for(int column = 1 ; column <= 7 ; column ++){
        PlayNumber[column]=GetValue(column);
    }     
    for(int column = 1 ; column <= 7 ; column++)
    {
        if(PlayNumber[column] != 0)
        {
            input[PlayNumber[column]]=XO;
            if(winning() != 0)
            {
                PlayOut ++;
                if(XO=='O'){
                    EVA ++;

                } else {
                    EVA --;
                }    
                input[PlayNumber[column]]=' ';
                return -1;
            }
            input[PlayNumber[column]]=' ';
        }
    }
    if(Depth <= 1)
    {
        for(int column = 1 ; column <= 7 ; column ++)
        {
            int temp=0;
            if( PlayNumber[column] != 0 )
            {
                input[PlayNumber[column]]=XO;
                if( winning() != 0 )
                {
                    PlayOut++;
                    if(XO=='O')
                    {
                        EVA++;
                    }    
                    else
                    {
                        EVA--;
                    }    
                    input[PlayNumber[column]]=' ';
                    return -1;
                }
                temp = NegaMax(Depth+1);
                if(column == 1){
                    chance = temp;}
                if(chance < temp){
                    chance = temp;}
                input[PlayNumber[column]]=' ';
            }
        }    
    }
    return -chance;

}

void screensaver(){

  for(int i = 0; i < 7; i++){ 
        for (int j = 0; j <= 7; j++){
          leds[j*7 + i] = CRGB :: Green;
          leds[j*7 + i-1] = CRGB :: Yellow;
        
        }
      if (i == 3){
          leds[60] = CRGB :: Green;
        } else {
          leds[60] = CRGB :: Black;
        }

        if (i == 4){
          leds[58] = CRGB :: Yellow;
        } else {
          leds[58] = CRGB :: Black;
        }

        if (i == 2){
          leds[56] = CRGB :: Yellow;
        } else {
          leds[56] = CRGB :: Black;
        }
        FastLED.show();
        delay(80);
        for (int j = 0; j <= 7; j++){
          leds[j*7 + i] = CRGB :: Black;
          leds[j*7 + i-1] = CRGB :: Black;
          leds[58] = CRGB :: Black;
          leds[60] = CRGB :: Black;

        }

        FastLED.show();
        delay(10);
  }
  for(int i = 0; i < 8; i++){ 
        for (int j = 0; j < 7; j++){
          input[j + 7*i] = 'O';
        }
        Board(0);
        }


  for(int i = 0; i < 7; i++){ 
        leds[colcds[i]] = CRGB :: Green;
        }
        FastLED.show();
        delay(200);
  for(int i = 0; i < 7; i++){ 
        leds[colcds[i]] = CRGB :: Black;
        }
        FastLED.show();
}



