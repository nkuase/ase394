/*
  keyestudio Electronic scale
  lesson 2.1
  PWM
  http://www.keyestudio.com
*/
#define LED 10 //define a pin of LED as D10
int value;
void setup()
{
  pinMode(LED, OUTPUT);// initialize digital pin LED as an output.
}
void loop () {
  for (value = 0; value < 255; value = value + 1) {
    analogWrite (LED, value); // LED lights gradually light up
    delay (5); // delay 5MS
  }
  for (value = 255; value > 0; value = value - 1) {
    analogWrite (LED, value); // LED gradually goes out
    delay (5); // delay 5MS
  }
}
