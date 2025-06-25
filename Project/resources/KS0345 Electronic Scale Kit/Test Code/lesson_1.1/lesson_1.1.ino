/*
  keyestudio Electronic scale
  lesson 1.1
  Blink
  http://www.keyestudio.com
*/
#define LED  10 // define a pin of LED as D10

void setup()
{
  pinMode(LED, OUTPUT);// initialize digital pin LED as an output.
}

void loop() // the loop function runs over and over again forever
{
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level
  delay(1000);    // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);    // wait for a second
}
