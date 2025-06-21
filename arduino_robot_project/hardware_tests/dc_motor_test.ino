const int in1 = 6;
const int in2 = 7;
const int ena = 10;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
}

void loop() {
  analogWrite(ena, 200);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(2000);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(2000);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(1000);
}
