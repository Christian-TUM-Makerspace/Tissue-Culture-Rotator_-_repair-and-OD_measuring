// Testaufbau mit 300 kOhm zwischen 2 und 5.
// 4 nicht belegt
// Siehe Datenblatt 8.3.2.1 Changing Responsivity

const int sensorPin = A1;
int sensorVal = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(sensorPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  sensorVal = analogRead(sensorPin);
  Serial.println(sensorVal);
  delay(5); //bei genau 5 ms bzw 200 Hz gleichmäßig überlagerte Frequenzen

}
