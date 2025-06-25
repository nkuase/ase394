/*
  keyestudio Electronic scale
  Lesson_5.1
  Keypad
  http://www.keyestudio.com
*/

#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keypressed;
//define the cymbols on the buttons of the keypads
char keyMap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4,5}; //Row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //Column pinouts of the keypad
Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

void setup()
{
  Serial.begin(9600); //Set baud rate to 9600
}

void loop ()
{
  keypressed = myKeypad.getKey();  // read key values
  if (keypressed != NO_KEY) { //read key values
    Serial.print("key:");
    Serial.println(keypressed);   //print key values
  }
}
