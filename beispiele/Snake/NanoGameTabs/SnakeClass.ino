#include <Coordinates.h>
#include <EEPROM.h>
//defines fuer die Richtung in die die Schlange kriecht
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
//LCD Aufloesung
#define X_LCD_RESOLUTION 84
#define Y_LCD_RESOLUTION 48

#define BLOCK_SIZE 3                      //groesse der Spielfeldbloecke

#define X_RESOLUTION X_LCD_RESOLUTION / BLOCK_SIZE    // 84 / 3  = 28
#define Y_RESOLUTION Y_LCD_RESOLUTION / BLOCK_SIZE    // 48 / 3  = 16

#define VERIFICATION 0xff234597           //testen, ob bereits in den EEPROM geschrieben wurde
//############################################################################################################################################################################
class Snake {
  private:
    const uint16_t eeAddress = 4;         //die ersten 4 Byte signalisieren ob bereits eine Liste da ist
    uint32_t highscoreList[3];            //highscoreliste
    uint32_t points;                      //Punktestand
//food variablen
    Coordinates foodPosition;             //enthaelt die koordinaten des essens
    bool noFood;                          //gibt es noch essen?
    uint16_t foodTime;                    //timer fuer das essen der Schlange

    uint8_t storedBrightness;
    uint16_t blinkTime;
    bool blinkBL;
    
    uint16_t moveTime;                    //timer fuer die Bewegung der Schlange
    uint8_t moveDirection;                //signalisiert die Richtung in die die Schlange kriecht
//Positionen des Kopfes, des Schwanzes und aller anderen Segmente
    Coordinates headPosition;
    Coordinates tailPosition;
    Coordinates segmente[X_RESOLUTION*Y_RESOLUTION];
    uint16_t tailIndex;                   //aktuelles Array-Ende
//Timeouts
    const uint16_t moveTimeout = 180;     //Zeit bis sich die Schlange um ein Segment nach vorn bewegt, in millisekunden
    const uint16_t foodTimeout = 6000;    //Zeit bis das Futter verschwindet und neues erscheint, in millisekunden
    const uint16_t blinkBLtimeout = 500;
  
  public:
  Snake() {}
//main der Klasse Snake, wird kontinuierlich ausgefuehrt
  runningGame(){
    printField();
    if(moveTime >= moveTimeout) {                                   //Muss sich die Schlange fortbewegen?
      moveTime = 0;
      moveSnake();
    }
    extendSnake();                                                  //hat die Schlange etwas gefressen?
    changeDirection();                                              //Wurde die Richtung gewechselt?
    if(foodTime >= foodTimeout || noFood) {                         //War das Futter lang genug da oder wurde es gefressen?
      foodTime = 0;
      randomSeed(TCNT1);
      foodGenerator();
    }
    if(isSnakeCrossing() || tasten.getButtonCycle(buttonSelect)) {  //kreuzt sich die Schlange? Will der Nutzer das Spiel beenden?
      gameOver();
      startGame();
    }
    if(blinkBL && (blinkTime >= blinkBLtimeout)) {
      BRIGHTNESS = storedBrightness;
      blinkBL = false;
    }
  }
//Spiel vorbei
  gameOver() {
    clearField();
    printField();
    lcd.printOut("Game over!", 1, 11);
    lcd.writeRect(0,9,83,2,true);
    lcd.printOut("Punkte:", 3, 0);
    lcd.printOut(String(points, DEC), 5, 0);
    lcd.renderAll();
    highscore();      //high score ggf. speichern/abrufen
//warte auf Start-Taste    
    tasten.clearAllButtons();
    while(!tasten.getButtonCycle(buttonStart)) {
    }
    tasten.clearAllButtons();
//high scores ausgeben
    clearField();
    printField();
    lcd.printOut("High scores", 1, 10);
    lcd.writeLine(0,9,83,9);
    for(uint8_t i = 0; i < 3; i++) { 
      lcd.printOut(String(i+1, DEC) + ". " + String(highscoreList[i],DEC), i+3, 0);
    }
    lcd.renderAll(); 
//warte auf Start-Taste    
    tasten.clearAllButtons();
    while(!tasten.getButtonCycle(buttonStart)) {
    }
    tasten.clearAllButtons();
  }
//Spiel initialisieren
  startGame() {
    blinkBL = false;
    points = 0;
    noFood = false;
    foodTime = 0;
    moveTime = 0;
//LCD loeschen und von vorn
    lcd.clear(false);
    lcd.writeRect(0,6,82,11,false);
    lcd.printOut("Druecke Start",2, 2);
    lcd.renderLine();
    while(!tasten.getButtonCycle(buttonStart)) {
    }
    randomSeed(TCNT1);
    clearField();
    tailIndex = 3;
    headPosition.setCoordinates(14,8);    //Position Kopf festlegen
    segmente[0] = headPosition;           //Schattenkopie
    segmente[1].setCoordinates(15,8);     //zweites Segment
    segmente[2].setCoordinates(16,8);     //drittes Segment"
    foodGenerator();
    moveDirection = LEFT;                 //Startrichtung links
    printSnake();                         //Schlange ausgeben
    tasten.clearAllButtons();
    foodTime = 0;                         //food timer auf 0
    moveTime = moveTimeout;               //move timer auf 0 ACHTUNG! BEENDET SPIEL SOFORT!!!
  }
//blink
inline blinkBacklight() {
  if(!blinkBL) {
    storedBrightness = BRIGHTNESS;
    BRIGHTNESS = 255;
    blinkBL = true;
    blinkTime = 0;
  }
}
//highscore speichern und Liste laden
  highscore() {
    long verify;
    bool store = false;
    EEPROM.get(0, verify);
    if(verify == VERIFICATION) {
      EEPROM.get(eeAddress, highscoreList);
      for(uint8_t i = 0; i < 3; i++) {
        if(highscoreList[i] < points) {
          highscoreList[i] = points;
          i = 3;
          store = true;
        }
      }
    } else {
        for(uint8_t i = 1; i < 3; i++) {
          highscoreList[i] = 0;
        }
        highscoreList[0] = points;
      EEPROM.put(0, VERIFICATION);
    }
    if(store) EEPROM.put(eeAddress, highscoreList);
  }  
//pruefen ob Schlange sich kreuzt
  bool isSnakeCrossing() {
    bool crossing = false;
    for(uint16_t i = 0; i < tailIndex; i++) {
      if( (segmente[i].getX() == headPosition.getX()) && (segmente[i].getY() == headPosition.getY()) ) {
          crossing = true;
        }
    }
    return crossing;
  }
//Schlange erweitern
  extendSnake() {
    if((foodPosition.getX() == headPosition.getX()) && (foodPosition.getY() == headPosition.getY())) {
      tailIndex++;
      blinkBacklight();
      for(int16_t i = tailIndex - 1; i > 0; i--) {
        segmente[i] = segmente[i-1];
      }
      points += foodTime;
      noFood = true;
    }
  }
//Richtung wechseln
  changeDirection() {
    if(tasten.checkButton(buttonDown) && (moveDirection != UP)) {
      moveDirection = DOWN;
    }
    if(tasten.checkButton(buttonUp) && (moveDirection != DOWN)) {
      moveDirection = UP;
    }
    if(tasten.checkButton(buttonRight) && (moveDirection != LEFT)) {
      moveDirection = RIGHT;
    }
    if(tasten.checkButton(buttonLeft) && (moveDirection != RIGHT)) {
      moveDirection = LEFT;
    }
  }
//die Schlange bewegen
  moveSnake() {
    switch(moveDirection) {
      case UP:    moveUp();
                  break;
      case DOWN:  moveDown();
                  break;
      case LEFT:  moveLeft();
                  break;
      case RIGHT: moveRight();
                  break;
      default:    break;
      
    }
    moveSegments();
    setBlock(foodPosition.getX(), foodPosition.getY());  // Futter erneut zeichnen
  }
  inline moveLeft() {
    segmente[0] = headPosition;
    if(headPosition.getX() > 0) {
      headPosition.setCoordinates(headPosition.getX() - 1, headPosition.getY()); 
    } else {
      headPosition.setCoordinates(X_RESOLUTION - 1, headPosition.getY());
    }
  }
  inline moveRight() {
    segmente[0] = headPosition;
    if(headPosition.getX() < (X_RESOLUTION - 1)) {
      headPosition.setCoordinates(headPosition.getX() + 1, headPosition.getY()); 
    } else {
      headPosition.setCoordinates(0, headPosition.getY());
    }
  }
  inline moveUp(){
    segmente[0] = headPosition;
    if(headPosition.getY() > 0) {
      headPosition.setCoordinates(headPosition.getX(), headPosition.getY() - 1); 
    } else {
      headPosition.setCoordinates(headPosition.getX(), Y_RESOLUTION - 1);
    }
  }
  inline moveDown() {
    segmente[0] = headPosition;
    if(headPosition.getY() < Y_RESOLUTION - 1) {
      headPosition.setCoordinates(headPosition.getX(), headPosition.getY() + 1); 
    } else {
      headPosition.setCoordinates(headPosition.getX(), 0);
    }
  }
//Alle Segmente nachruecken lassen
  moveSegments() {
    for(int16_t i = tailIndex - 1; i > 0; i--) {
      segmente[i] = segmente[i-1];
    }
    printSnake();
  }
//Schlange zeichnen
  printSnake() {
    clearField(); //Spielfeld vorab loeschen
    for(int16_t i = 0; i < tailIndex; i++) {
      setBlock(segmente[i].getX(), segmente[i].getY());
    }
    setBlock(headPosition.getX(), headPosition.getY());   //Kopf zeichnen
  }
//Spielfeld veraendern
  setBlock(uint8_t x, uint8_t y) {    //Block zeichnen
    for(uint8_t i = 0; i < BLOCK_SIZE; i++) {
      for(uint8_t j = 0; j < BLOCK_SIZE; j++) {
        setXY(x * BLOCK_SIZE + i, y * BLOCK_SIZE + j);
      }      
    }
  }  
  clearBlock(uint8_t x, uint8_t y) {  //Block loeschen
    for(uint8_t i = 0; i < BLOCK_SIZE; i++) {
      for(uint8_t j = 0; j < BLOCK_SIZE; j++) {
        clearXY(x * BLOCK_SIZE + i, y * BLOCK_SIZE + j);
      }      
    }
  }
//loescht Spielfeld
  inline clearField() {
    lcd.clear(false);           //kein rendern
  }
//gibt Spielfeld auf LCD aus
  inline printField() {
    lcd.renderAll();
  }
//LCD stuff
//Pixel setzen
  inline setXY(uint8_t x, uint8_t y) {
   lcd.setPixel(x,y,1);
  }
//Pixel loeschen
  inline clearXY(uint8_t x, uint8_t y) {
   lcd.setPixel(x,y,0);
  }
//Futter erzeugen
  foodGenerator() {
    noFood = false;
    bool insideSnake = true;
    uint8_t tempX;
    uint8_t tempY;
    while(insideSnake) {                              //neues Futter immernoch innerhalb der Schlange?
      insideSnake = false;
      tempX = (uint8_t)random(X_RESOLUTION - 1);      //dann erzeuge Futter an anderer Stelle
      tempY = (uint8_t)random(Y_RESOLUTION - 1);      //
      if((headPosition.getX() == tempX) && (headPosition.getY() == tempY)) {
        insideSnake = true;
      }
      for(uint16_t i = 0; i < tailIndex; i++) {
        if((segmente[i].getX() ==  tempX) && (segmente[i].getY() ==  tempY)) {
          insideSnake = true;
        }
      }
    }
    foodPosition.setCoordinates(tempX, tempY);    //lege neues Futter aus
  }
  inline increaseTimer() {
    foodTime++;
    moveTime++;
    blinkTime++;
  }
};
//############################################################################################################################################################################
