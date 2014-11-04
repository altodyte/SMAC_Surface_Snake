// Library Inclusions

// Pinout
int led = 13; // 12 is the USER LED on the Arduino Uno.
int throttlePin = 11; // Throttle PWM Input | [Action tied to Throttle]
int rudderPin = 10; // Rudder PWM Input | [Action tied to Rudder]
// System Parameters

// STATE VARIABLES
// RC state variables
int throttlePWM; // current throttle pwm width reading
int rudderPWM; // current rudder pwm width reading
// Action state variables
int actTest = 0, actMin = -100, actMax = 100;
// Blinking State Variables
boolean lightState = true;
int blinkPeriod = 500;
unsigned long lastBlink = millis();

// System Parameters
int pwmMin = 1150, pwmMax = 1850; // Might need to be updated

// Object Instances

void setup() {
  Serial.begin(9600);
  // Initialize Pins
  pinMode(throttlePin, INPUT);
  pinMode(rudderPin, INPUT);
  pinMode(led, OUTPUT); 
  // Other Set-up
}

void loop() {
  // Read PWM
  throttlePWM = pulseIn(throttlePin, HIGH);
  rudderPWM = pulseIn(rudderPin, HIGH);

  // Mapping PWM to Actions
  // actTest = pwmToRange(coerce(throttlePWM,pwmMin,pwmMax),actMin,actMax);
  actTest = pwmToRange(actTest,actMin,actMax);
  
  // Command actions

  // Print Status for Debugging
  Serial.println("Throttle: "+String(throttlePWM)+
                 ", Rudder: "+String(rudderPWM)+
                 "|| Test A: "+String(actTest));

  // Handle Heartbeat Blinking
  if (millis()>lastBlink+blinkPeriod){
    lightState = !lightState;
    digitalWrite(led,lightState);
    lastBlink = millis();
  }  
}

// Mapping Functions
int coerce(int in, int rangeMin, int rangeMax){
  // Bound integer input to range determined by min and max
  if (in<rangeMin){ return rangeMin; }
  else if (in>rangeMax){ return rangeMax; }
  else { return in; }
}

int pwmToRange(int pwm, int rangeMin, int rangeMax){
  long a1 = coerce(throttlePWM,pwmMin,pwmMax);
  long a2 = a1-pwmMin;
  long b1 = pwmMax-pwmMin;
  long a3 = a2*(rangeMax - rangeMin);
  long a4 = int(float(a3)/(b1));
  return a4+actMin;
}