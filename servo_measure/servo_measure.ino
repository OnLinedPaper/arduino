/*
 * measures arduino's built-in servo
 * appears to cycle at 2800Hz
 */

//==================================================================

  #include <Servo.h>

  Servo extra;
  int sensor = 2;
  unsigned long i;
  unsigned long low_counts;
  unsigned long high_counts;
  unsigned long cycles;
  float low_time;
  float high_time;

  float low_rat;  //running average low time
  float high_rat; //running average high time

  unsigned long micros_per_cycle;
  unsigned long cycles_per_second;

  unsigned long average_hz;
  
  bool high = false;

  unsigned long start_time;
  unsigned long end_time;
  int prev_time;
  float avg_time;
  float placeholder;

//==================================================================

void setup() {

  pinMode(sensor, INPUT);
  digitalWrite(sensor, LOW);

  pinMode(LED_BUILTIN, OUTPUT);

  i = 0;
  low_counts = 0;
  high_counts = 0;
  cycles = 0;
  low_time = 0;
  high_time = 0;

  micros_per_cycle =     200;   //note that in theory a servo is
                                //supposed to run with 20ms 
                                //(20000us) PWM, and that the 
                                //shortest pulses may be as small 
                                //as 1ms (1000us) - note also that 
                                //the program takes an average of 
                                //~50us per loop; it is therefore 
                                //recommended to run between 50 and 
                                //1000 micros_per_cycle
  cycles_per_second = 1000000 / micros_per_cycle;

  start_time = 0;
  end_time = 0;
  prev_time = 0;
  avg_time = 0;
  placeholder = 0;
  
  extra.attach(3);
  extra.write(0);
  
  Serial.begin(9600);

}

//==================================================================

void loop() {

  delayMicroseconds(micros_per_cycle - prev_time);  //delay between measurements
                                                                  //take measurement time into account

  start_time = micros();
  //if(bitRead(PIND, sensor) == 1) {
  if(digitalRead(sensor) == HIGH) {   //what's more reliable - bitRead or digitalRead? what's faster?
    digitalWrite(LED_BUILTIN, HIGH);
    high_counts++;
    
    if(high == false) {
      //just came off a low cycle
      cycles++;
    }
    high = true;
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    low_counts++;
    
    high = false;
  }
  
  if(i % cycles_per_second == 0) {
    //every "second", print out high counts and low counts
    //note that high measurement rates will lead to delayed printouts

    calculate_pwm();
    printout();
    
    high_counts = 0;
    low_counts = 0;
    high_time = 0;
    low_time = 0;
    cycles = 0;
  }
  end_time = micros();
  prev_time = end_time - start_time;
  
  placeholder = prev_time;
  avg_time = (avg_time * (i%cycles_per_second) + prev_time) / ((i%cycles_per_second)+1);

  i++;
  
}

//========================================================================================

void printout()
{
    //print out relevant data to measurements
    
    Serial.print("measuring at ");
    Serial.print(micros_per_cycle);
    Serial.print(" microsecond intervals and ");
    Serial.print(cycles_per_second);
    Serial.print(" measurements per second (");
    Serial.print(i);
    Serial.print(" measurements since program start)\n");
    
    Serial.print(high_counts);
    Serial.print(" measurements were high\n"); 
     
    Serial.print(low_counts);
    Serial.print(" measurements were low\n");  
    
    Serial.print(cycles);
    Serial.print(" cycles this second\n"); 

    Serial.print(avg_time);
    Serial.print(" average micros per loop\n");
    
    Serial.print("\n");  
}

//==============================================================================

void calculate_pwm()
{
  //calculates the pulse-width modulation of the measured servo by dividing 
  //number of times measured high and low by the measurements

  high_time = high_counts / i;
  low_time = low_counts / i;
}

//==============================================================================


