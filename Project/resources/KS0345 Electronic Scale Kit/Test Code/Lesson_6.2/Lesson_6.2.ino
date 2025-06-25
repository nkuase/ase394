/*
  keyestudio Electronic scale
  Lesson_6.2
  DS3231
  http://www.keyestudio.com
*/
#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

DS3231 clock;

bool century = false; ///true is 22century, set true to 22th century
bool h12Flag; //we default false and use 24 hour format
bool pmFlag;  //set variable to false and set 24 hour format

byte year, month, date, hour, minute, second, week; //save the variable of time

void setup() {

  lcd.init();          // initialize LCD
  lcd.backlight();     //set backlight of LCD on

  // Start the I2C interface
  Wire.begin();

  //  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("2");
  lcd.setCursor(1, 0);
  if (century) {
    lcd.print("1");
  } else {
    lcd.print("0");
  }

}

void loop() {
  // send what's going on to the LCD1602.
  lcd.setCursor(2, 0);
  year = clock.getYear();
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
