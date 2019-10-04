#include "shiftRegButtonLib.h"

 shiftRegButton::shiftRegButton() {

  //init Pins
  shiftDatPORT &= ~shiftDatPin;
  shiftClkPORT &= ~shiftClkPin;

  //shiftInDDR &= ~shiftInPin;
  shiftDatDDR |= shiftDatPin;
  shiftClkDDR |= shiftClkPin;

  shiftInPORT |= shiftInPin;

  //init all flags
  shiftDatHi;
  for(uint8_t i = 0; i < numberOfButtons; i++, shiftClk()) {
   buttonsPressed[i] = false;   
   buttonsCycle[i] = false;  
   buttonsTime[i] = 0;
   cycleFlag[i] = false;
   //init shiftreg outputs to all 1's
  }
 }

 shiftRegButton::checkButtons(void) {
  //this procedure is needed by the 74HC595, other shift registers' behaviour may differ. See the datasheet of your part for more informations.
  shiftDatLo;	//
  shiftClk();	//zero to shift stage 0
  shiftDatHi;	//
  shiftClk();	//zero to parallel output 0
  for(uint8_t i = 0; i < numberOfButtons; i++, shiftClk()) {
    //is button i actually pressed?
    if(buttonsInput()) {
      buttonsPressed[i] = true;
      cycleFlag[i] = true;
      buttonsTime[i]++;
    } else {
       buttonsPressed[i] = false;
       buttonsTime[i] = 0;
    }
    checkButtonCycle(i);
    //increase the time button i is pressed, or clear if it is not pressed anymore
  }
 }

 bool shiftRegButton::checkButton(uint8_t n) {
  return buttonsPressed[n];
 }
 
 shiftRegButton::checkButtonCycle(uint8_t n) {
  if(!buttonsPressed[n] && cycleFlag[n]) {
   buttonsCycle[n] = true;
   cycleFlag[n] = false;
  }
 }
 
 shiftRegButton::clearButton(uint8_t buttonToBeCleared) {
   buttonsCycle[buttonToBeCleared] = false;
 }
 
 shiftRegButton::clearAllButtons() {
  for(uint8_t i = 0; i < numberOfButtons; i++) {  
   buttonsCycle[i] = false;
  }
 }

 bool shiftRegButton::getButtonCycle(uint8_t n) {
   return buttonsCycle[n];
 }
