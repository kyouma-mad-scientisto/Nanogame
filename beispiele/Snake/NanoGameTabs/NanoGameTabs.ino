//template
#include <PCD8544_SPI.h>
#include <shiftRegButtonLib.h>
#include <myInterrupts.h>
#include "DFRobotDFPlayerMini.h"

#define CLR_BIT(p,n) ((p) &= ~((1) << (n)))
#define SET_BIT(p,n) ((p) |= (1 << (n)))

#define BL 10         //backlight, Hintergrundbeleuchtung LCD
#define SD_CARD_CS 2  //D2 ist Chip Enable
#define BAT_VOLTAGE A7 //Akkuspannung an A7
#define _OE_LEVEL 7   //D7 fuer Output Enable Pegelwandler
#define BRIGHTNESS OCR1B

#define SOUND

//globale Objekte anlegen
volatile PCD8544_SPI_FB lcd;
volatile myInterrupts Timer2;
volatile myInterrupts pwmBL;
volatile shiftRegButton tasten;
#ifdef SOUND
DFRobotDFPlayerMini myDFPlayer;
#endif
