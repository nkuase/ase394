/*
  keyestudio Electronic scale
  Lesson_8.1
  HX711
  http://www.keyestudio.com
*/
#include <Keypad.h>
#include <HX711.h>
#include <EEPROM.h>
#include <math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


HX711 hx(12, 13, 128);
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keypressed;  //used to receive key values
//define the cymbols on the buttons of the keypads
char keyMap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};    //Row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9};    //Column pinouts of the keypad
Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

double ratio, offset;//define two variables, ratio is scale coefficient
boolean KE, KF;  //press flag


void setup() {
  // put your setup code here, to run once:
  lcd.init();          // initialize LCD
  lcd.backlight();     //set the backlight of LCD to on
  //set the read offset

  lcd.init();
  lcd.print("Welcome! HX711"); //show start page
  delay(2000);
  EEPROM.get(0, ratio);
  //EEPROM.get(8, offset); // read the offset from EEPROM 
  offset = hx.tare();
  hx.set_offset(offset);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Weight: ");
  lcd.setCursor(15, 0);
  lcd.print("g");
}

void loop() {
  //press key to get key value place the corresponding flag on 1
  keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY) {
    if (keypressed == '*') {
      KE = 1;
    }else if (keypressed == '#') {
      KF = 1; 
    }
  }

  if (KE == 1) {  //press * to calibrate
    KE = 0;
    calibrate();  //The screen shows that 50g balance wight should be put on. After a while, release the key and wait for calibration, then enter the weighing page and show 50g.The error can be within 1g
  }
  if (KF == 1) {  //press # to deduct tare. The screen will show 0, if 0 doesn't appear on screen 
    KF = 0;
    offset = hx.tare();    // subtract tare and read the offset
    hx.set_offset(offset);  // set the read offset
    //EEPROM.put(8, offset);
  }
  
  double sum = 0;
  char sbuff[6];
  sum = hx.bias_read() * ratio;
  sprintf(sbuff, "%6d", (long)sum);
  lcd.setCursor(8, 0);
  lcd.print(sbuff);        //show the read weight
  delay(100);
}

void calibrate() {
  lcd.clear();
  double sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += hx.bias_read();
  }
  sum = sum / 10;
  lcd.clear();
  lcd.print("PUT ON 50g Farmar");
  lcd.setCursor(0, 1);
  lcd.print("9 seconds later");
  int countdown = 9;
  while (countdown != 0) {
    lcd.setCursor(0, 1);
    lcd.print(countdown);
    countdown--;
    delay(1000);
  }
  double sum1 = 0;
  for (int i = 0; i < 10; i++) {
    sum1 += hx.bias_read();
  }
  sum1 = sum1 / 10;
  ratio = abs(sum1 - sum);
  ratio = 50 / ratio;
  EEPROM.put(0, ratio);//save coefficient which needs compiling via above 1.6.0 IDE
  lcd.clear();
  lcd.print("CAL OK!");
  // set the read offset

  lcd.init();
  lcd.print("Welcome! HX711"); //show the activation page
  delay(2000);
  EEPROM.get(0, ratio);
  //EEPROM.get(8, offset); //  read offset from EEPROM
  offset = hx.tare();
  hx.set_offset(offset);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Weight: ");
  lcd.setCursor(15, 0);
  lcd.print("g");
}
