// Reagenzglasläufer

// Christian Schormair christianschormair@gmx.de christian.schormair@tum.de


//Sensor optical Opt101, orange cable
#define sensorPin 32
int sensorVal = 0;

//Hall Sensor
const int hallSensor = 33;
int hallVal = 0;

// Define stepper motor connections:
#define dirPin 18
const int stepPin = 19; //black cable (blue)


//Steppermotor
//https://randomnerdtutorials.com/esp32-pwm-arduino-ide/
//https://www.makerguides.com/tb6600-stepper-motor-driver-arduino-tutorial/


void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  pinMode(hallSensor, INPUT);
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
