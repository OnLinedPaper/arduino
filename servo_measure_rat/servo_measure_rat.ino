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

  float low_rat;    //running average low time
  float high_rat;   //running average high time
  float cycle_rat;  //running average cycles
  float rat_count;  //number of times we've added to the running average total

  unsigned long micros_per_measurement;
  unsigned long measurements_per_second;

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

  i = 1;  //start i at 1 so it doesn't immediately trigger a pwm printout
  low_counts = 0;
  high_counts = 0;
  cycles = 0;

  micros_per_measurement =     200;   //note that in theory a servo is
                                //supposed to run with 20ms 
                                //(20000us) PWM, and that the 
                                //shortest pulses may be as small 
                                //as 1ms (1000us) - note also that 
                                //the program takes an average of 
                                //~50us per loop; it is therefore 
                                //recommended to run between 50 and 
                                //1000 micros_per_measurement
  measurements_per_second = 1000000 / micros_per_measurement;

  start_time = 0;
  end_time = 0;
  prev_time = 0;
  avg_time = 0;
  placeholder = 0;

  low_rat = 0;
  high_rat = 0;
  
  extra.attach(3);
  extra.write(180);
  
  Serial.begin(9600);

}

//==================================================================

void loop() {

  delayMicroseconds(micros_per_measurement);  //delay between measurements

//-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
//main measure section
  
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

//-  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
//calculate pwm and print in this section

  if(i % measurements_per_second == 0) {
    //every "second", print out high counts and low counts
    //note that high measurement rates will lead to delayed printouts

    
    calculate_pwm();
    printout();
    
    high_counts = 0;
    low_counts = 0;
    cycles = 0;
  }

  i++;
  
}

//========================================================================================

void printout()
{
    //print out relevant data to measurements

    float low_r = low_rat / rat_count;
    float high_r = high_rat / rat_count;
    float cycle_r = cycle_rat / rat_count;

    Serial.print("--------------------------------------------------------------------------------------------------------\n\n");
    
    Serial.print("measuring at ");
    Serial.print(micros_per_measurement);
    Serial.print(" microsecond intervals and ");
    Serial.print(measurements_per_second);
    Serial.print(" measurements per second (");
    Serial.print(i);
    Serial.print(" measurements since program start)\n");
    
    Serial.print(high_counts);
    Serial.print(" measurements were high\n"); 
     
    Serial.print(low_counts);
    Serial.print(" measurements were low\n");  
     
    Serial.print(cycles);
    Serial.print(" cycles\n");  

    Serial.print("\n");
    

    Serial.print(low_r);
    Serial.print(" low running average total measurements per cycle\n");

    Serial.print(high_r);
    Serial.print(" high running average total measurements per cycle\n");

    Serial.print(cycle_r);
    Serial.print(" running average total cycles per measurement\n");

    
    Serial.print(high_r / (high_r + low_r), 4);
    Serial.print(" : ");
    Serial.print(low_r / (high_r + low_r), 4);
    Serial.print("\n");
    
    Serial.print("\n");  
}

//==============================================================================

void calculate_pwm()
{
  //calculates the pulse-width modulation of the measured servo by dividing 
  //number of times measured high and low by the measurements
  low_rat += low_counts;
  high_rat += high_counts;
  cycle_rat += cycles;
  
  rat_count += 1;
  
}

//==============================================================================


