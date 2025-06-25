/*
  keyestudio Electronic scale
  Lesson_9
  Electronic scale
  http://www.keyestudio.com
*/

#include <Keypad.h>
#include <DS3231.h>
#include <HX711.h>
#include <EEPROM.h>
#include <math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

DS3231 clock;
HX711 hx(12, 13, 128);
double ratio, offset;//define two variables, ratio is scalefactor,
double weight = 0, P = 0, M = 0, D = 0;
int P1, P2, P3, P4, D1, D2, D3, D4, N;
//The two LEDs are connected to D10 and D11
const int LED1 = 10;
const int LED2 = 11;

bool century = false;
bool h12Flag;
bool pmFlag;
byte alarmDay, alarmHour1, alarmMinute1, alarmSecond1, alarmBits = 0x48;
byte alarmHour2, alarmMinute2;
bool alarmDy = false, alarmH12Flag = false, alarmPmFlag = false;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keypressed;  //used to receive the key vlaue
float key_num;    //press keys to show numbers
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

const int buzzer = A0;
byte row_k = 0, col_k = 0, page = 0;
//bool change_flag  = false;
boolean K0, K1, K2, K3, K4, K5, K6, K7, K8, K9, KA, KB, KC, KD, KE, KF;  //key flag
byte year, month, date, hour, minute, second, week;
int y1, y2, mon1, mon2, d1, d2, h1, h2, min1, min2, s1, s2;
int alarm_h1, alarm_h2, alarm_m1, alarm_m2, alarm_s1, alarm_s2;
int alarm_hh1, alarm_hh2, alarm_mm1, alarm_mm2;
bool BC_flag;   //false selective value plus 1, true sselective value subtracts 1
bool scale_flag = false;  //false is pricing scale,true is counting scale is counting scale


void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  lcd.init();          // initialize LCD
  lcd.backlight();     //set the backlight of LCD on

  // Start the I2C interface
  Wire.begin();

  // Start the serial interface
  Serial.begin(57600);

}

void loop() {

  /* press 'D' key to switch page
     when page is 0,display data and page
     when page is 1, show the page of alarm and setting
     when page is 2, show the page of electronic scale and setting page
  */

  if (page == 0) {
    showDatePage();
  }
  else if (page == 1) {
    showAlarmPage();
  }
  else if (page == 2) {
    showScalePage();
  }


}


/****************Subprogram**************************/
void showDatePage(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("2");
  lcd.setCursor(1, 0);
  if (century) {
    lcd.print("1");
  } else {
    lcd.print("0");
  }
  while (true) {
    alarm();         //detect alarm
    showDate();      //show date
    enter();         //input
    if (KA == 1)     //time delayed, move cursor
    {
      KA = 0;        //key flag, clear 0
      while (true) {
        enter();     //input
        if (KA == 1) {
          KA = 0;
          row_k += 1;
          if (row_k > 15) {
            row_k = 0;
            col_k += 1;
            if (col_k > 1) {
              col_k = 0;
            }
          }
        }
        lcd.setCursor(row_k, col_k);
        lcd.blink();

        if (KB == 1) {      // the value plus 1
          KB = 0;           //key flag, clear 0
          BC_flag = false;
          if (col_k == 0) { //the first row
            changeOne();
          } else {
            changeTwo();    //the second row
          }
        }
        if (KC == 1) {      //the vlaue subtracts 1
          KC = 0;           //key flag, clear 0
          BC_flag = true;
          if (col_k == 0) { //the first row
            changeOne();
          } else {
            changeTwo();    //the second row
          }
        }
        if (KF == 1) {
          KF = 0;           //key flag, clear 0
          setTime();
          break;
        }
      }
      KD = 0;   //D is invalid before exiting
    }
    if (KD == 1) {
      KD = 0;
      page = 1;
      break;
    }
  }

}


void showDate() {
  // send what's going on to the LCD1602.
  lcd.setCursor(2, 0);
  year = clock.getYear();
  y1 = year / 10;
  y2 = year % 10;
  if (year < 10) {
    lcd.setCursor(2, 0);
    lcd.print("0");
    lcd.setCursor(3, 0);
    lcd.print(year);
  } else {
    lcd.setCursor(2, 0);
    lcd.print(year);
  }

  lcd.setCursor(4, 0);
  lcd.print("-");

  // then the month
  month = clock.getMonth(century);
  mon1 = month / 10;
  mon2 = month % 10;
  if (month < 10) {
    lcd.setCursor(5, 0);
    lcd.print("0");
    lcd.setCursor(6, 0);
    lcd.print(month);
  } else {
    lcd.setCursor(5, 0);
    lcd.print(month);
  }

  lcd.setCursor(7, 0);
  lcd.print("-");

  // then the date
  date = clock.getDate();
  d1 = date / 10;
  d2 = date % 10;
  if (date < 10) {
    lcd.setCursor(8, 0);
    lcd.print("0");
    lcd.setCursor(9, 0);
    lcd.print(date);
  } else {
    lcd.setCursor(8, 0);
    lcd.print(date);
  }

  lcd.setCursor(10, 0);
  lcd.print(" ");

  // and the day of the week
  week = clock.getDoW();
  lcd.setCursor(11, 0);
  lcd.print("Week");
  lcd.setCursor(15, 0);
  lcd.print(week);

  // Finally the hour, minute, and second
  hour = clock.getHour(h12Flag, pmFlag);
  h1 = hour / 10;
  h2 = hour % 10;
  if (hour < 10) {
    lcd.setCursor(0, 1);
    lcd.print("0");
    lcd.setCursor(1, 1);
    lcd.print(hour);
  } else {
    lcd.setCursor(0, 1);
    lcd.print(hour);
  }

  lcd.setCursor(2, 1);
  lcd.print(":");

  minute = clock.getMinute();
  min1 = minute / 10;
  min2 = minute % 10;
  if (minute < 10) {
    lcd.setCursor(3, 1);
    lcd.print("0");
    lcd.setCursor(4, 1);
    lcd.print(minute);
  } else {
    lcd.setCursor(3, 1);
    lcd.print(minute);
  }

  lcd.setCursor(5, 1);
  lcd.print(":");

  second = clock.getSecond();
  s1 = second / 10;
  s2 = second % 10;
  if (second < 10) {
    lcd.setCursor(6, 1);
    lcd.print("0");
    lcd.setCursor(7, 1);
    lcd.print(second);
  } else {
    lcd.setCursor(6, 1);
    lcd.print(second);
  }
  lcd.setCursor(8, 1);
  lcd.print(" ");

  // Display the temperature
  if (clock.getTemperature() < 10) {
    lcd.setCursor(11, 1);
    lcd.print(" ");
    lcd.setCursor(12, 1);
    lcd.print(clock.getTemperature(), 1);
  } else {
    lcd.setCursor(11, 1);
    lcd.print(clock.getTemperature(), 1);
  }
  lcd.setCursor(15, 1);
  lcd.print("C");
}

void showAlarmPage(void) {
  lcd.clear();
  while (true) {
    showAlarmStatus();   //display the status of alarm
    enter();
    KB = 0;   //B is invalid before A is pressed
    KC = 0;   //C is invalid before A is pressed
    if (KA == 1)         // check alarm, move cursor
    {
      KA = 0;
      row_k = 9;
      col_k = 0;
      while (true) {
        enter();
        if (KA == 1) {
          KA = 0;
          col_k += 1;
          if (col_k > 1) {
            col_k = 0;
          }
        }
        lcd.setCursor(row_k, col_k);
        lcd.blink();

        if (KB == 1) {
          KB = 0;
          switch (col_k) {
            case 0:
              showAlarm1();
              row_k = 4;
              col_k = 1;
              while (true) {
                enter();
                if (KA == 1) {
                  KA = 0;
                  row_k += 1;
                  if (row_k > 11) {
                    row_k = 4;
                  }
                }
                lcd.setCursor(row_k, col_k);
                lcd.blink();
                if (KB == 1) {
                  KB = 0;
                  BC_flag = false;
                  changeAlarmOne();
                }
                if (KC == 1) {
                  KC = 0;
                  BC_flag = true;
                  changeAlarmOne();
                }
                if (KF == 1) {
                  KF = 0;
                  clock.turnOnAlarm(1);
                  //alarm1Flag = true;
                  break;
                }
              }
              row_k = 9;
              col_k = 0;
              lcd.clear();
              showAlarmStatus();
              break;

            case 1:
              showAlarm2();
              row_k = 4;
              col_k = 1;
              while (true) {
                enter();
                if (KA == 1) {
                  KA = 0;
                  row_k += 1;
                  if (row_k > 8) {
                    row_k = 4;
                  }
                }
                lcd.setCursor(row_k, col_k);
                lcd.blink();
                if (KB == 1) {
                  KB = 0;
                  BC_flag = false;
                  changeAlarmTwo();
                }
                if (KC == 1) {
                  KC = 0;
                  BC_flag = true;
                  changeAlarmTwo();
                }
                if (KF == 1) {
                  KF = 0;
                  clock.turnOnAlarm(2);
                  //alarm2Flag = true;
                  break;
                }
              }
              row_k = 9;
              col_k = 0;
              lcd.clear();
              showAlarmStatus();
              break;

            default: break;
          }
        }
        if (KC == 1) {
          KC = 0;
          switch (col_k) {
            case 0:
              clock.turnOffAlarm(1);
              row_k = 9;
              col_k = 0;
              //lcd.clear();
              showAlarmStatus();
              break;

            case 1:
              clock.turnOffAlarm(2);
              row_k = 9;
              col_k = 1;
              //lcd.clear();
              showAlarmStatus();
              break;

            default: break;
          }
        }
        if (KF == 1) {
          KF = 0;
          break;
        }

      }
      KD = 0;   //D is invalid before # is pressed
    }
    if (KD == 1) {
      KD = 0;
      break;
    }
    page = 2;
  }

}

void showAlarmStatus() {
  lcd.setCursor(0, 0);
  lcd.print("Alarm 1:");
  if (clock.checkAlarmEnabled(1)) {
    lcd.setCursor(9, 0);
    lcd.print("Y");
  } else {
    lcd.setCursor(9, 0);
    lcd.print("N");
  }

  lcd.setCursor(0, 1);
  lcd.print("Alarm 2:");
  if (clock.checkAlarmEnabled(2)) {
    lcd.setCursor(9, 1);
    lcd.print("Y");
  } else {
    lcd.setCursor(9, 1);
    lcd.print("N");
  }
}

void showAlarm1() {
  clock.getA1Time(alarmDay, alarmHour1, alarmMinute1, alarmSecond1, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Set Alarm 1   ");

  alarm_h1 = alarmHour1 / 10;
  alarm_h2 = alarmHour1 % 10;
  if (alarmHour1 < 10) {
    lcd.setCursor(4, 1);
    lcd.print("0");
    lcd.setCursor(5, 1);
    lcd.print(alarmHour1);
  } else {
    lcd.setCursor(4, 1);
    lcd.print(alarmHour1);
  }

  lcd.setCursor(6, 1);
  lcd.print(":");

  alarm_m1 = alarmMinute1 / 10;
  alarm_m2 = alarmMinute1 % 10;
  if (alarmMinute1 < 10) {
    lcd.setCursor(7, 1);
    lcd.print("0");
    lcd.setCursor(8, 1);
    lcd.print(alarmMinute1);
  } else {
    lcd.setCursor(7, 1);
    lcd.print(alarmMinute1);
  }

  lcd.setCursor(9, 1);
  lcd.print(":");

  alarm_s1 = alarmSecond1 / 10;
  alarm_s2 = alarmSecond1 % 10;
  if (alarmSecond1 < 10) {
    lcd.setCursor(10, 1);
    lcd.print("0");
    lcd.setCursor(11, 1);
    lcd.print(alarmSecond1);
  } else {
    lcd.setCursor(10, 1);
    lcd.print(alarmSecond1);
  }
}

void showAlarm2() {
  clock.getA2Time(alarmDay, alarmHour2, alarmMinute2, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Set Alarm 2   ");

  alarm_hh1 = alarmHour2 / 10;
  alarm_hh2 = alarmHour2 % 10;
  if (alarmHour2 < 10) {
    lcd.setCursor(4, 1);
    lcd.print("0");
    lcd.setCursor(5, 1);
    lcd.print(alarmHour2);
  } else {
    lcd.setCursor(4, 1);
    lcd.print(alarmHour2);
  }

  lcd.setCursor(6, 1);
  lcd.print(":");

  alarm_mm1 = alarmMinute2 / 10;
  alarm_mm2 = alarmMinute2 % 10;
  if (alarmMinute2 < 10) {
    lcd.setCursor(7, 1);
    lcd.print("0");
    lcd.setCursor(8, 1);
    lcd.print(alarmMinute2);
  } else {
    lcd.setCursor(7, 1);
    lcd.print(alarmMinute2);
  }

}

void showScalePage() {
  EEPROM.get(0, ratio);
  //EEPROM.get(8, offset); // read the offset from EEPROM 
  offset = hx.tare();
  hx.set_offset(offset);

  while (page == 2) {
    if (!scale_flag) {
      lcd.clear();
      while (true) {
        showScaleOne();
        enter();
        if (KA == 1) {
          KA = 0;
          row_k = 2;
          col_k = 1;
          while (true) {
            enter();     //input
            if (KA == 1) {
              KA = 0;
              row_k += 1;
              if (row_k > 6) {
                row_k = 2;
              }
            }
            lcd.setCursor(row_k, col_k);
            lcd.blink();
            changeScaleOne();
            if (KF == 1) {
              KF = 0;
              break;
            }
          }
        }

        if (KD == 1) {
          KD = 0;
          page = 0;
          break;
        }
        if (KE == 1) {  // press * to calibrate
          KE = 0;
          calibrate();  //The screen shows that 50g balance wight should be put on. After a while, release the key and wait for calibration, then enter the weighing page and show 50g.The error can be within 1g
        }
        if (KF == 1) {  //press # to deduct tare. The screen will show 0, if 0 doesn't appear on screen 
          KF = 0;
          offset = hx.tare();    // subtract tare and read the offset
          hx.set_offset(offset);  // set the read offset
          //EEPROM.put(8, offset);
        }
        if (KC == 1) {
          KC = 0;
          scale_flag = !scale_flag;
          break;
        }
      }

    } else {
      lcd.clear();
      while (true) {
        showScaleTwo();
        enter();
        if (KA == 1) {
          KA = 0;
          row_k = 2;
          col_k = 1;
          while (true) {
            enter();     //input
            if (KA == 1) {
              KA = 0;
              row_k += 1;
              if (row_k > 6) {
                row_k = 2;
              }
            }
            lcd.setCursor(row_k, col_k);
            lcd.blink();
            changeScaleTwo();
            if (KF == 1) {
              KF = 0;
              break;
            }
          }
        }
        if (KD == 1) {
          KD = 0;
          page = 0;
          break;
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
        if (KC == 1) {
          KC = 0;
          scale_flag = !scale_flag;
          break;
        }
      }
    }

  }

}

void showScaleOne() {
  weight = hx.bias_read() * ratio;
  weight /= 1000.0;
  char buff1[6];
  dtostrf(weight, 6, 3, buff1);
  lcd.setCursor(4, 0);
  lcd.print(buff1);
  lcd.setCursor(10, 0);
  lcd.print("kg");

  lcd.setCursor(0, 1);
  lcd.print("P=");
  lcd.setCursor(5, 1);
  lcd.print(".");

  P1 = int(P * 10) / 1000;
  P2 = (int(P * 10) - P1 * 1000) / 100;
  P3 = (int(P * 10) - P1 * 1000 - P2 * 100) / 10;
  P4 = int(P * 10) % 10;
  lcd.setCursor(2, 1);
  lcd.print(P1);
  lcd.setCursor(3, 1);
  lcd.print(P2);
  lcd.setCursor(4, 1);
  lcd.print(P3);
  lcd.setCursor(6, 1);
  lcd.print(P4);

  lcd.setCursor(8, 1);
  lcd.print("M=");
  M = weight * P;
  char buff3[6];
  dtostrf(M, -6, 1, buff3); //right alignment
  if (M >= 0) {
    lcd.setCursor(10, 1);
    lcd.print(buff3);
  }
  else {
    lcd.setCursor(10, 1);
    lcd.print("----.-");
  }
}

void showScaleTwo() {
  weight = hx.bias_read() * ratio;
  weight /= 1000.0;
  char buff1[6];
  dtostrf(weight, 6, 3, buff1);
  lcd.setCursor(4, 0);
  lcd.print(buff1);
  lcd.setCursor(10, 0);
  lcd.print("kg");

  lcd.setCursor(0, 1);
  lcd.print("D=");
  lcd.setCursor(5, 1);
  lcd.print(".");

  D1 = int(D * 10) / 1000;
  D2 = (int(D * 10) - D1 * 1000) / 100;
  D3 = (int(D * 10) - D1 * 1000 - D2 * 100) / 10;
  D4 = int(D * 10) % 10;
  lcd.setCursor(2, 1);
  lcd.print(D1);
  lcd.setCursor(3, 1);
  lcd.print(D2);
  lcd.setCursor(4, 1);
  lcd.print(D3);
  lcd.setCursor(6, 1);
  lcd.print(D4);


  lcd.setCursor(9, 1);
  lcd.print("N=");
  N = weight * 1000 / D;
  if (N >= 0) {
    if (N < 10) {
      lcd.setCursor(11, 1);
      lcd.print(N);
      lcd.setCursor(12, 1);
      lcd.print("    ");
    } else if (N < 100) {
      lcd.setCursor(11, 1);
      lcd.print(N);
      lcd.setCursor(13, 1);
      lcd.print("   ");
    } else if (N < 1000) {
      lcd.setCursor(11, 1);
      lcd.print(N);
      lcd.setCursor(14, 1);
      lcd.print("  ");
    }
    else if (N < 10000) {
      lcd.setCursor(11, 1);
      lcd.print(N);
      lcd.setCursor(15, 1);
      lcd.print(" ");
    } else {
      lcd.setCursor(11, 1);
      lcd.print(N);
    }
  }
  else {
    lcd.setCursor(11, 1);
    lcd.print("-----");
  }
}

void changeScaleOne() {
  if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ) {
    key_num = keypressed - '0';
    switch (row_k) {
      case 2:
        lcd.setCursor(row_k, col_k);
        lcd.print(keypressed);
        P = P - P1 * 100 + key_num * 100;
        break;

      case 3:
        lcd.setCursor(row_k, col_k);
        lcd.print(keypressed);
        P = P - P2 * 10 + key_num * 10;
        break;

      case 4:
        lcd.setCursor(row_k, col_k);
        lcd.print(keypressed);
        P = P - P3 + key_num;
        break;

      case 6:
        lcd.setCursor(row_k, col_k);
        lcd.print(keypressed);
        P = P - float(P4) / 10 + key_num / 10;
        break;

      default : break;
    }
  }
}

void changeScaleTwo() {
  if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
      keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
      keypressed == '8' || keypressed == '9' ) {
    key_num = keypressed - '0';
    switch (row_k) {
      case 2:
        lcd.setCursor(row_k, col_k);
        lcd.print(keypressed);
        D = D - D1 * 100 + key_num * 100;
        break;

      case 3:
        lcd.setCursor(row_k, col_k);
        lcd.print(keypressed);
        D = D - D2 * 10 + key_num * 10;
        break;

      case 4:
        lcd.setCursor(row_k, col_k);
        lcd.print(keypressed);
        D = D - D3 + key_num;
        break;

      case 6:
        lcd.setCursor(row_k, col_k);
        lcd.print(keypressed);
        D = D - float(D4) / 10 + key_num / 10;
        break;

      default : break;
    }
  }
}

void calibrate() {
  lcd.init();   //initialize LCD1602, avoid cursor flashing
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
    tone(buzzer, 700, 100);
    delay(1000);
  }
  lcd.setCursor(0, 1);
  lcd.print("Release key now");
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
  EEPROM.get(0, ratio);
  //EEPROM.get(8, offset); // read offset from EEPROM
  offset = hx.tare();
  hx.set_offset(offset);
  lcd.clear();
}

void changeOne() {    //modify year, month, day, week at the first row
  switch (row_k) {    //select row
    case 2:
      if (!BC_flag) {
        y1 += 1;
      } else {
        y1 -= 1;
      }
      if (y1 > 9) y1 = 0;
      if (y1 < 0) y1 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(y1);
      year = y1 * 10 + y2;
      break;

    case 3:
      if (!BC_flag) {
        y2 += 1;
      } else {
        y2 -= 1;
      }
      if (y2 > 9) y2 = 0;
      if (y2 < 0) y2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(y2);
      year = y1 * 10 + y2;
      break;

    case 5:
      if (!BC_flag) {
        mon1 += 1;
      } else {
        mon1 -= 1;
      }
      if (mon1 > 1) mon1 = 0;
      if (mon1 < 0) mon1 = 1;
      lcd.setCursor(row_k, col_k);
      lcd.print(mon1);
      month = mon1 * 10 + mon2;
      break;

    case 6:
      if (!BC_flag) {
        mon2 += 1;
      } else {
        mon2 -= 1;
      }
      if (mon2 > 9) mon2 = 0;
      if (mon2 < 0) mon2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(mon2);
      month = mon1 * 10 + mon2;
      break;

    case 8:
      if (!BC_flag) {
        d1 += 1;
      } else {
        d1 -= 1;
      }
      if (d1 > 3) d1 = 0;
      if (d1 < 0) d1 = 3;
      lcd.setCursor(row_k, col_k);
      lcd.print(d1);
      date = d1 * 10 + d2;
      break;

    case 9:
      if (!BC_flag) {
        d2 += 1;
      } else {
        d2 -= 1;
      }
      if (d2 > 9) d2 = 0;
      if (d2 < 0) d2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(d2);
      date = d1 * 10 + d2;
      break;

    case 15:
      if (!BC_flag) {
        week += 1;
      } else {
        week -= 1;
      }
      if (week > 7) week = 1;
      if (week < 1) week = 7;
      lcd.setCursor(row_k, col_k);
      lcd.print(week);
      break;

    default:  break;
  }
}

void changeTwo() {    //modify year, month, day, week at the second row
  switch (row_k) {    //select column
    case 0:
      if (!BC_flag) {
        h1 += 1;
      } else {
        h1 -= 1;
      }
      if (h1 > 2) h1 = 0;
      if (h1 < 0) h1 = 2;
      lcd.setCursor(row_k, col_k);
      lcd.print(h1);
      hour = h1 * 10 + h2;
      break;

    case 1:
      if (!BC_flag) {
        h2 += 1;
      } else {
        h2 -= 1;
      }
      if (h2 > 9) h2 = 0;
      if (h2 < 0) h2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(h2);
      hour = h1 * 10 + h2;
      break;

    case 3:
      if (!BC_flag) {
        min1 += 1;
      } else {
        min1 -= 1;
      }
      if (min1 > 5) min1 = 0;
      if (min1 < 0) min1 = 5;
      lcd.setCursor(row_k, col_k);
      lcd.print(min1);
      minute = min1 * 10 + min2;
      break;

    case 4:
      if (!BC_flag) {
        min2 += 1;
      } else {
        min2 -= 1;
      }
      if (min2 > 9) min2 = 0;
      if (min2 < 0) min2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(min2);
      minute = min1 * 10 + min2;
      break;

    case 6:
      if (!BC_flag) {
        s1 += 1;
      } else {
        s1 -= 1;
      }
      if (s1 > 5) s1 = 0;
      if (s1 < 0) s1 = 5;
      lcd.setCursor(row_k, col_k);
      lcd.print(s1);
      second = s1 * 10 + s2;
      break;

    case 7:
      if (!BC_flag) {
        s2 += 1;
      } else {
        s2 -= 1;
      }
      if (s2 > 9) s2 = 0;
      if (s2 < 0) s2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(s2);
      second = s1 * 10 + s2;
      break;

    default:  break;
  }
}

void changeAlarmOne() {
  switch (row_k) {
    case 4:
      if (!BC_flag) {
        alarm_h1 += 1;
      } else {
        alarm_h1 -= 1;
      }
      if (alarm_h1 > 2) alarm_h1 = 0;
      if (alarm_h1 < 0) alarm_h1 = 2;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_h1);
      alarmHour1 = alarm_h1 * 10 + alarm_h2;
      break;

    case 5:
      if (!BC_flag) {
        alarm_h2 += 1;
      } else {
        alarm_h2 -= 1;
      }
      if (alarm_h2 > 9) alarm_h2 = 0;
      if (alarm_h2 < 0) alarm_h2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_h2);
      alarmHour1 = alarm_h1 * 10 + alarm_h2;
      break;

    case 7:
      if (!BC_flag) {
        alarm_m1 += 1;
      } else {
        alarm_m1 -= 1;
      }
      if (alarm_m1 > 5) alarm_m1 = 0;
      if (alarm_m1 < 0) alarm_m1 = 5;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_m1);
      alarmMinute1 = alarm_m1 * 10 + alarm_m2;
      break;

    case 8:
      if (!BC_flag) {
        alarm_m2 += 1;
      } else {
        alarm_m2 -= 1;
      }
      if (alarm_m2 > 9) alarm_m2 = 0;
      if (alarm_m2 < 0) alarm_m2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_m2);
      alarmMinute1 = alarm_m1 * 10 + alarm_m2;
      break;

    case 10:
      if (!BC_flag) {
        alarm_s1 += 1;
      } else {
        alarm_s1 -= 1;
      }
      if (alarm_s1 > 5) alarm_s1 = 0;
      if (alarm_s1 < 0) alarm_s1 = 5;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_s1);
      alarmSecond1 = alarm_s1 * 10 + alarm_s2;
      break;

    case 11:
      if (!BC_flag) {
        alarm_s2 += 1;
      } else {
        alarm_s2 -= 1;
      }
      if (alarm_s2 > 9) alarm_s2 = 0;
      if (alarm_s2 < 0) alarm_s2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_s2);
      alarmSecond1 = alarm_s1 * 10 + alarm_s2;
      break;

    default: break;
  }
  clock.setA1Time(alarmDay, alarmHour1, alarmMinute1, alarmSecond1, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
}

void changeAlarmTwo() {
  switch (row_k) {
    case 4:
      if (!BC_flag) {
        alarm_hh1 += 1;
      } else {
        alarm_hh1 -= 1;
      }
      if (alarm_hh1 > 2) alarm_hh1 = 0;
      if (alarm_hh1 < 0) alarm_hh1 = 2;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_hh1);
      alarmHour2 = alarm_hh1 * 10 + alarm_hh2;
      break;

    case 5:
      if (!BC_flag) {
        alarm_hh2 += 1;
      } else {
        alarm_hh2 -= 1;
      }
      if (alarm_hh2 > 9) alarm_hh2 = 0;
      if (alarm_hh2 < 0) alarm_hh2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_hh2);
      alarmHour2 = alarm_hh1 * 10 + alarm_hh2;
      break;

    case 7:
      if (!BC_flag) {
        alarm_mm1 += 1;
      } else {
        alarm_mm1 -= 1;
      }
      if (alarm_mm1 > 5) alarm_mm1 = 0;
      if (alarm_mm1 < 0) alarm_mm1 = 5;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_mm1);
      alarmMinute2 = alarm_mm1 * 10 + alarm_mm2;
      break;

    case 8:
      if (!BC_flag) {
        alarm_mm2 += 1;
      } else {
        alarm_mm2 -= 1;
      }
      if (alarm_mm2 > 9) alarm_mm2 = 0;
      if (alarm_mm2 < 0) alarm_mm2 = 9;
      lcd.setCursor(row_k, col_k);
      lcd.print(alarm_mm2);
      alarmMinute2 = alarm_mm1 * 10 + alarm_mm2;
      break;

    default: break;
  }
  clock.setA2Time(alarmDay, alarmHour2, alarmMinute2, alarmBits, alarmDy, alarmH12Flag, alarmPmFlag);
}

void alarm() {
  // Indicate whether an alarm went off
  if (clock.checkIfAlarm(1)) {    //clock 1 detects alarm
    tone(buzzer, 2000);
  }

  if (clock.checkIfAlarm(2)) {    //clock 2 detects alarm
    tone(buzzer, 1000);
  }

}

//set year, month, day, week, hour, minute and second of chip
void setTime() {
  clock.setSecond(second);  //set second
  clock.setMinute(minute);  //set minute
  clock.setHour(hour);      //set hour
  clock.setDoW(week);       //set week
  clock.setDate(date);      //set day
  clock.setMonth(month);    //set month
  clock.setYear(year);      //set year

}

//input, obtain the key vlaues
void enter() {
  keypressed = myKeypad.getKey();
  if (keypressed != NO_KEY) {
    switch (keypressed) {
      case '1' :  tone(buzzer, 262, 100);  digitalWrite(LED1, HIGH); break;

      case '2' :  tone(buzzer, 293, 100);  digitalWrite(LED2, HIGH); break;

      case '3' :  tone(buzzer, 329, 100);  break;

      case '4' :  tone(buzzer, 349, 100);  break;

      case '5' :  tone(buzzer, 392, 100);  break;

      case '6' :  tone(buzzer, 440, 100);  break;

      case '7' :  tone(buzzer, 494, 100);  break;

      case '8' :  tone(buzzer, 523, 100);  break;

      case '9' :  tone(buzzer, 586, 100);  break;

      case '0' :  tone(buzzer, 697, 100);  digitalWrite(LED1, LOW); digitalWrite(LED2, LOW);   break;

      case 'A' :  tone(buzzer, 1045, 100); KA = 1; break;

      case 'B' :  tone(buzzer, 879, 100);  KB = 1; break;

      case 'C' :  tone(buzzer, 987, 100);  KC = 1; break;

      case 'D' :  tone(buzzer, 1971, 100); KD = 1; break;

      case '*' :  tone(buzzer, 658, 100);  KE = 1;  break;

      case '#' :  tone(buzzer, 783, 100);  KF = 1; break;

      default:  break;
    }
  }
}
