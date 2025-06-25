/*
  keyestudio Electronic scale
  Lesson_4.2
  buzzer
  http://www.keyestudio.com
*/
const int buzzer 13 //buzzer pin to D13
void setup() {
  pinMode(buzzer, OUTPUT);//set digital 13 to OUTPUT
}

void loop () {
  birthday();
}

////////////////////////Set Happy Birthday//////////////////////////////////
void birthday()
{
  tone(buzzer, 294); //buzzer outputs a sound with 294Hz
  delay(250);//delay in 250ms
  tone(buzzer, 440);
  delay(250);
  tone(buzzer, 392);
  delay(250);
  tone(buzzer, 532);
  delay(250);
  tone(buzzer, 494);
  delay(500);
  tone(buzzer, 392);
  delay(250);
  tone(buzzer, 440);
  delay(250);
  tone(buzzer, 392);
  delay(250);
  tone(buzzer, 587);
  delay(250);
  tone(buzzer, 532);
  delay(500);
  tone(buzzer, 392);
  delay(250);
  tone(buzzer, 784);
  delay(250);
  tone(buzzer, 659);
  delay(250);
  tone(buzzer, 532);
  delay(250);
  tone(buzzer, 494);
  delay(250);
  tone(buzzer, 440);
  delay(250);
  tone(buzzer, 698);
  delay(375);
  tone(buzzer, 659);
  delay(250);
  tone(buzzer, 532);
  delay(250);
  tone(buzzer, 587);
  delay(250);
  tone(buzzer, 532);
  delay(500);
}
