#pragma once

#ifndef myInterrupts_h
#define myInterrupts_h
#include <Arduino.h>

class myInterrupts {
 public:
	myInterrupts();
	initTimer1();
	disableTimer1Interrupt();
	enableTimer1Interrupt();
	
	initTimer2();
	disableTimer2Interrupt();
	enableTimer2Interrupt();
	
	initOCR1B();
};

#endif
