#define hallSensor 32

int hallVal = 0;

void setup() {
  Serial.begin(115200);
  pinMode(hallSensor, INPUT);

}

void loop() {
  hallVal = analogRead(hallSensor);
  Serial.println(hallVal);
  delay(100);

}
