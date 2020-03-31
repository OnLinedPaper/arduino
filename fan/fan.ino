/*
 * yellow spinning controller
 * spins a servo yellow
 */

 #include <Servo.h>

/*
 int red = 2; //red control pin
 Servo fan;
 int deg = 0;
*/
 Servo extra;


  int red = 2;
  unsigned long i;
  unsigned long low_counts;
  unsigned long high_counts;

void setup() {

  pinMode(red, INPUT);
  digitalWrite(red, LOW);

  pinMode(LED_BUILTIN, OUTPUT);

  i = 0;
  low_counts = 0;
  high_counts = 0;

/*
  pinMode(red, OUTPUT); //output pin to servo

  fan.attach(red);
  fan.write(0);
*/
  extra.attach(3);
  extra.write(180);

}

void loop() {

  if(digitalRead(red)) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  delayMicroseconds(10); //100,000 measurements per second
  i++;

/*
  delay(1000);
  fan.write(deg);

  deg = deg + 10;
  if(deg >= 360)
  {
    //deg = 0;
  }
*/

  /*
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(yellow, LOW);
  delay(100);
  
  digitalWrite(yellow, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  */
  
}
