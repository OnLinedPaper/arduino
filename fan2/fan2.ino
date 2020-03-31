//wires to fan: 
//cpu_yellow and cpu_green are PWM control for CPU
//gpu_yellow and gpu_green are PWM control for GPU
int cpu_yellow = 6; 
int cpu_green = 5; 
int gpu_yellow = 10;
int gpu_green = 9;


//push buttons
int cpu_reset = 4;
int cpu_speedUp = 3;
int cpu_slowDown = 2;
int gpu_reset = 13;
int gpu_speedUp = 12;
int gpu_slowDown = 11;

//don't dip below operating ranges
int cpu_fanSpeed = 90;
int gpu_fanSpeed = 90;
int minFanSpeed = 20;
int maxFanSpeed = 250;

//only register initial button click
bool cpu_speedButton = false;
bool cpu_slowButton = false;
bool gpu_speedButton = false;
bool gpu_slowButton = false;


void setup() {


  pinMode(cpu_yellow, OUTPUT);
  pinMode(cpu_green, OUTPUT);
  pinMode(gpu_yellow, OUTPUT);
  pinMode(gpu_green, OUTPUT);

  pinMode(cpu_reset, INPUT);
  pinMode(cpu_speedUp, INPUT);
  pinMode(cpu_slowDown, INPUT);
  pinMode(gpu_reset, INPUT);
  pinMode(gpu_speedUp, INPUT);
  pinMode(gpu_slowDown, INPUT);


  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(cpu_yellow, HIGH);
  digitalWrite(cpu_green, LOW);
  digitalWrite(gpu_yellow, HIGH);
  digitalWrite(gpu_green, LOW);

  digitalWrite(LED_BUILTIN, LOW);

  //set frequency to remove whining noise from fan
  setPwmFrequency(cpu_yellow, 1);
  setPwmFrequency(cpu_green, 1);
  setPwmFrequency(gpu_yellow, 1);
  setPwmFrequency(gpu_green, 1);
  
  analogWrite(cpu_yellow, 0);
  analogWrite(cpu_green, 0);
  analogWrite(gpu_yellow, 0);
  analogWrite(gpu_green, 0);


}

void loop() {

  if(digitalRead(cpu_speedUp)){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }


  delay(100);
  
  writeFan();

  if(digitalRead(cpu_reset)) {
    cpu_startup();
  }
  
  if(digitalRead(gpu_reset)) {
    gpu_startup();
  }

  
  if(digitalRead(cpu_speedUp)){
    if(!cpu_speedButton) {
      cpu_speedButton = true;
      if(cpu_fanSpeed < maxFanSpeed) {
        cpu_fanSpeed += 10;
      }
    }
  }
  else {
    cpu_speedButton = false;
  }
  
  if(digitalRead(gpu_speedUp)){
    if(!gpu_speedButton) {
      gpu_speedButton = true;
      if(gpu_fanSpeed < maxFanSpeed) {
        gpu_fanSpeed += 10;
      }
    }
  }
  else {
    gpu_speedButton = false;
  }

  
  if(digitalRead(cpu_slowDown)){
    if(!cpu_slowButton) {
      cpu_slowButton = true;
      if(cpu_fanSpeed > minFanSpeed) {
        cpu_fanSpeed -= 10;
      }
    }
  }
  else {
    cpu_slowButton = false;
  }
  
  if(digitalRead(gpu_slowDown)){
    if(!gpu_slowButton) {
      gpu_slowButton = true;
      if(gpu_fanSpeed > minFanSpeed) {
        gpu_fanSpeed -= 10;
      }
    }
  }
  else {
    gpu_slowButton = false;
  }

  
  
  
}


//write current speed to fan
void writeFan() {
  analogWrite(cpu_yellow, cpu_fanSpeed);
  analogWrite(cpu_green, cpu_fanSpeed);
  analogWrite(gpu_yellow, gpu_fanSpeed);
  analogWrite(gpu_green, gpu_fanSpeed);
}


//start low and give the fans a chance to spin up
void cpu_startup() {
  cpu_fanSpeed = 20;
}

void gpu_startup() {
  gpu_fanSpeed = 20;
}


//==================================================================
//https://playground.arduino.cc/Code/PwmFrequency


void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


