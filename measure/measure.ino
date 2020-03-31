
int mPin = 2; //measure pin
int val = 0;
unsigned long i = 0;


void setup() {
  pinMode(mPin, INPUT);

  Serial.begin(9600);

}

void loop() {

  //delay(1);
  delayMicroseconds(100);
  
  if(i%10000 == 0) {
    Serial.print(i);
    Serial.print("\n");
  }
  i++;
}
