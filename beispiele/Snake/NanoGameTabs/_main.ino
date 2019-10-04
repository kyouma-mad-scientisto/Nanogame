#include <avr/interrupt.h>
#include <avr/io.h>
#include <Arduino.h>
//############################################################################################################################################################################

//Snake Instanz anlegen
Snake snake;

void setup() {
//Timer2 fuer 1ms initialisieren
  Timer2.initTimer2();
  Timer2.enableTimer2Interrupt();
//Hintergrundbeleuchtung pwm
  pwmBL.initOCR1B();
//Pegelwandler ein
  pinMode(_OE_LEVEL, OUTPUT);
  digitalWrite(_OE_LEVEL, HIGH);
//Hintergrundbeleuchtung an, halbe Helligkeit  
  pinMode(BL, OUTPUT);
  BRIGHTNESS = 127;
//LCD init,   invert, vop, tempcoef, bias  
  lcd.begin(false, 0xB1, 0x04, 0x12);
  lcd.clear();
//erster Taster check
  tasten.checkButtons();
  Serial.begin(9600);   //set speed for Hardware  Serial pin 1 and 2 tx/rx
  myDFPlayer.begin(Serial);//select Serial to send comand to dfplayer
  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
  snake.startGame();
}
//Endlosschleife
void loop() {
  snake.runningGame();
}

ISR(TIMER2_COMPA_vect) { // called by timer2
  tasten.checkButtons();  //Aufruf 1-mal pro Millisekunde
  snake.increaseTimer();
}
