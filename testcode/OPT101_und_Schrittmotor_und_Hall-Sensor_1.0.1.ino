// Reagenzglasläufer

// Soweit funktionierender Testaufbau mit:
// ESP32 am PC angeschlossen
// Widerstand im 100-kOhm-Bereich zwischen 2 und 5.
// 4 nicht belegt
// Siehe Datenblatt 8.3.2.1 Changing Responsivity
// Klare gelbe (oder eher orange) LED
// 5 Volt oder 3,3 Volt 
// Siehe Datenblatt 8.3.2.1 Changing Responsivity

// Christian Schormair christianschormair@gmx.de


//Sensor Helligkeit Opt101, orange cable
#define sensorPin 32
int sensorVal = 0;


//LED, muss umgeschrieben werden, wenn man es benutzen will.
//Sonst Konflikt mit Motorsteuerung
//#define ledPin 32
//int freq = 100;
//int ledChannel = 0;
//int resolution = 8;
//int dutyCycle = 150; //maximal 255 bei resolution = 8 bit


//Hall Sensor
const int hallSensor = 33;
int hallVal = 0;


//Schrittmotor
//https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
//https://www.makerguides.com/tb6600-stepper-motor-driver-arduino-tutorial/

// Define stepper motor connections:
#define dirPin 18
const int stepPin = 19; //black cable (blue)

// setting PWM properties
const int freq = 50;
const int ledChannel = 0;
const int resolution = 8;
const int dutyCycle = 8;


void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  pinMode(hallSensor, INPUT);

//Schrittmotor mit PWM starten
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(stepPin, ledChannel);

  // Set the spinning direction CW/CCW:
  digitalWrite(dirPin, HIGH);
  ledcWrite(ledChannel, dutyCycle);

//LED mit PWM
//  ledcSetup(ledChannel, freq, resolution);
//  ledcAttachPin(ledPin, ledChannel);
//  ledcWrite(ledChannel, dutyCycle);
}

void loop() {

  sensorVal = analogRead(sensorPin);
//Serial.print("opt101: ");
Serial.print(sensorVal);
  Serial.println(""); //diese Zeie nach dem Testen löschen
//Serial.print("   ");

  hallVal = analogRead(hallSensor);
//  Serial.print("hall: ");
//  Serial.println(hallVal);
  delay(50);
}
