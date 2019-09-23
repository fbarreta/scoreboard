#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

//Pin connected to SH_CP of 74HC595
int clockPin1 = 13;
////Pin connected to DS of 74HC595
int dataPin1 = 12;
//Pin connected to ST_CP of 74HC595
int latchPin1 = 11;

//Pin connected to SH_CP of 74HC595
int clockPin2 = 10;
////Pin connected to DS of 74HC595
int dataPin2 = 9;
//Pin connected to ST_CP of 74HC595
int latchPin2 = 8;

//Pin connected to SH_CP of 74HC595
int clockPin3 = 7;
////Pin connected to DS of 74HC595
int dataPin3 = 6;
//Pin connected to ST_CP of 74HC595
int latchPin3 = 5;

//Pin connected to SH_CP of 74HC595
int clockPin4 = 4;
////Pin connected to DS of 74HC595
int dataPin4 = 3;
//Pin connected to ST_CP of 74HC595
int latchPin4 = 2;

int servLimit = 2;
int servCount = 0;
int server = 1;

const int SERVE1 = 15;
const int SERVE2 = 16;

const int RECEIVER = 14;
IRrecv irrecv(RECEIVER);
decode_results results;
int reading;
int score1 = 0;
int score2 = 0;
byte dig1_1 = 0;
byte dig1_2 = 0;
byte dig2_1 = 0;
byte dig2_2 = 0;
byte numbers[] = {
  B00011000,
  B11011110,
  B00110100,
  B10010100,
  B11010010,
  B10010001,
  B00010011,
  B11011100,
  B00010000,
  B11010000
};

void setup() {
  //Start Serial for debuging purposes  
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);  
  pinMode(clockPin1, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);  
  pinMode(clockPin2, OUTPUT);
  pinMode(latchPin3, OUTPUT);
  pinMode(dataPin3, OUTPUT);  
  pinMode(clockPin3, OUTPUT);
  pinMode(latchPin4, OUTPUT);
  pinMode(dataPin4, OUTPUT);  
  pinMode(clockPin4, OUTPUT);
  pinMode(SERVE1, OUTPUT);
  pinMode(SERVE2, OUTPUT);

}

void loop() {
  if(score1 > 99 || score1 < 0) {
    score1 = 0;
  }

  if(score2 > 99 || score2 < 0) {
    score2 = 0;
  }
  setNumber(score1, score2);
  if (irrecv.decode(&results)) {
      switch (results.value) {
        // UP PLAYER 1
        case 0xD7E84B1B:
          addScore1();
          break;
        // DOWN PLAYER 1
        case 0x97483BfB:
          subScore1();
          break;
        // RESET Player 1 and 2
        case 0xE318261B:
          reset();
          break;
         // UP PLAYER 2
        case 0xFF906F:
          addScore2();
          break;
        // DOWN PLAYER 2
        case 0xFFA857:
          subScore2();
          break;
        // Call serve 2
        case 0xFF30CF:
          reset();
          server = 2;
          break;
        // Call serve 1
        case 0x9716BE3F:
          reset();
          server = 1;
          break;
        default:
          break;
     }
     Serial.println (results.value, HEX); //display HEX results 
     irrecv.resume(); //next value
  }
  updateShiftRegister();
  checkServe();
}

void addScore1(){
  score1++;
  servCount++;
}

void subScore1(){
  score1--;
  servCount--;
}

void addScore2(){
  score2++;
  servCount++;
}

void subScore2(){
  score2--;
  servCount--;
}

void reset(){
  score1 = 0;
  score2 = 0;
  servCount = 0;
  servLimit = 2;
  server = 1;
}

void checkServe() {
  callServe(server);
  if (servCount == servLimit) {
    switchServe();jjjjkl
  }
  if ((score1 + score2) >= 8){
    servLimit = 1;
  }
}

void switchServe() {
  switch (server) {
    case 1:
      server = 2;
      break;
    case 2:
      server = 1;
      break;
    default:
      break;
  }
  servCount = 0;
}

void setNumber(int score1, int score2){
  int digit1 = (score1) % 10;
  int digit2 = (score1 / 10) % 10;
  int digit3 = (score2) % 10;
  int digit4 = (score2 / 10) % 10;
  dig1_1 = numbers[digit2];
  dig1_2 = numbers[digit1];
  dig2_1 = numbers[digit4];
  dig2_2 = numbers[digit3];
}

void callServe(int player) {
  if (player == 1) {
     digitalWrite(SERVE1, HIGH);
     digitalWrite(SERVE2, LOW);
  }
  if (player == 2) {
     digitalWrite(SERVE1, LOW);
     digitalWrite(SERVE2, HIGH);
  }
}

void updateShiftRegister()
{
   digitalWrite(latchPin1, LOW);
   digitalWrite(latchPin2, LOW);
   digitalWrite(latchPin3, LOW);
   digitalWrite(latchPin4, LOW);
   shiftOut(dataPin1, clockPin1, LSBFIRST, dig1_1);
   shiftOut(dataPin2, clockPin2, LSBFIRST, dig1_2);
   shiftOut(dataPin3, clockPin3, LSBFIRST, dig2_1);
   shiftOut(dataPin4, clockPin4, LSBFIRST, dig2_2);
   digitalWrite(latchPin1, HIGH);
   digitalWrite(latchPin2, HIGH);
   digitalWrite(latchPin3, HIGH);
   digitalWrite(latchPin4, HIGH);
}
