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
  snake.startGame();
}
//Endlosschleife
void loop() {
  snake.runningGame();
}

ISR(TIMER2_COMPA_vect) { // called by timer1
  tasten.checkButtons();  //Aufruf 1-mal pro Millisekunde
  snake.increaseTimer();
}
