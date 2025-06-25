/*
  keyestudio Electronic scale
  Lesson_4.1
  buzzer
  http://www.keyestudio.com
*/
const int buzzer A0 //buzzer pin to A0

void setup() {
  pinMode(buzzer, OUTPUT);//set digital A0 to OUTPUT
}

void loop () {
  tone(buzzer, 262);  //Buzzer emits a sound with 262Hz
  delay(250);   //delay in 250ms
  tone(buzzer, 294);;   //Buzzer emits a sound with 262Hz
  delay(250);   //delay in 250ms
  tone(buzzer, 330);
  delay(250);
  tone(buzzer, 349);
  delay(250);
  tone(buzzer, 392);
  delay(250);
  tone(buzzer, 440);
  delay(250);
  tone(buzzer, 494);
  delay(250);
  tone(buzzer, 532);
  delay(250);
  noTone(buzzer);   // Output sound when buzzer turns off
  delay(1000);
}
