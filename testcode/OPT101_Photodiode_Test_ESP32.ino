// Soweit funktionierender Testaufbau mit:
// ESP32 am PC
// 300 kOhm zwischen 2 und 5.
// 4 nicht belegt
// Siehe Datenblatt 8.3.2.1 Changing Responsivity
// Klare gelbe (oder eher orange) LED (kein Datenblatt)
// 5 Volt oder 3,3 Volt 
// Siehe Datenblatt 8.3.2.1 Changing Responsivity

// Christian Schormair christianschormair@gmx.de

//Sensor
#define sensorPin 33
int sensorVal = 0;

//LED
#define ledPin 32
int freq = 100;
int ledChannel = 0;
int resolution = 8;
int dutyCycle = 150; //maximal 255 bei resolution = 8 bit

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  ledcWrite(ledChannel, dutyCycle);
}

void loop() {

  sensorVal = analogRead(sensorPin);
  Serial.println(sensorVal);
  delay(5); //bei genau 5 ms bzw 200 Hz gibt es gleichmäßig überlagerte Frequenzen
}
