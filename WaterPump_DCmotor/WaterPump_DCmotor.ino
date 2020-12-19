//Jamie Quella
//2020.12.12

/************************
Automated water pump 5v DC 
motor using the L293D chip 
and water level sensor
************************/

// -- initialize DC motor
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// -- initialize water level sensor
int adc_id = 0;
char printBuffer[128];

// set up pins for motor 
void setup() {
  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  Serial.begin(9600);
}


// define vars for timing
const long readInterval = 200;
unsigned long previousMillis = 0;
signed long lastReadGap = 0;

// max amt of time for pump to run water
const long maxPumpInterval = 5000;
signed long intervalGap = 0;


// water level to stop/start motor pump
int stopLevel = 300;

// keep track of whether motor is currently on
bool running = false;

// function to turn motor on
void startMotor() {
  digitalWrite(ENABLE,HIGH); // full speed
//  analogWrite(ENABLE,225); // partial speed (max=255)
  digitalWrite(DIRA,HIGH); 
  digitalWrite(DIRB,LOW);  
}

// function to turn motor off
void stopMotor() {
  digitalWrite(ENABLE,LOW); 
  digitalWrite(DIRA,LOW); 
  digitalWrite(DIRB,LOW);
}


void loop() {
  int value = analogRead(adc_id); // get adc value
  
  unsigned long currentMillis = millis(); // read time
  lastReadGap = currentMillis - previousMillis; // how long it's been since last loop
  
  // read/act buffer - only execute if we've hit interval
  if (lastReadGap >= readInterval) {
    previousMillis = currentMillis;

    sprintf(printBuffer,"ADC %d level is %d\n",adc_id, value);
    Serial.print(printBuffer);   // print current value of water level

    // add to "motor on" timer if it's been running since last read
    if (running) {
      intervalGap += lastReadGap;
    }

    // turn pump on if water below required level
    if (value < stopLevel) {
      startMotor();
      running = true;
      Serial.println("GOGO");
    }
    // turn pump on if water below required level
    else { 
      Serial.println("STOP");
      stopMotor();
      running = false;
      intervalGap = 0; // reset "motor on" timer var
    }
    sprintf(printBuffer,"Motor running for: %d ms\n",intervalGap);
    Serial.print(printBuffer);

    // safety: turn off  motor if it's been running too long until we get another water level reading.
    // aka "don't flood the house"
    if (intervalGap > maxPumpInterval) {
      stopMotor();
    }
    
    Serial.println("");
  }
}
