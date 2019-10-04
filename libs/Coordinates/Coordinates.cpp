#pragma once
#include "Coordinates.h"

  Coordinates::Coordinates(){
    uint8_t xPosition = 0;
    uint8_t yPosition = 0;
  }
  Coordinates::Coordinates(uint8_t x, uint8_t y) {
    xPosition = x;
    yPosition = y;
  }
  Coordinates::getX(){
    return xPosition;
  }
  Coordinates::getY(){
    return yPosition;
  }
  Coordinates::setCoordinates(uint8_t x, uint8_t y) {
    xPosition = x;
    yPosition = y;
  }
