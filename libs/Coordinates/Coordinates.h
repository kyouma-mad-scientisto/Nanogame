#pragma once
#ifndef Coordinates_h
#define Coordinates_h
#include <Arduino.h>

class Coordinates {
  public:
  Coordinates();
  Coordinates(uint8_t x, uint8_t y);
  getX();
  getY();
  setCoordinates(uint8_t x, uint8_t y);

  private:
  uint8_t xPosition;
  uint8_t yPosition;
  
};

#endif
