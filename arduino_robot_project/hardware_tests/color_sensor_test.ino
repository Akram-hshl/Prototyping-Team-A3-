const int S0 = 2;
const int S1 = 3;
const int S2 = 4;
const int S3 = 5;
const int sensorOut = 12;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  Serial.begin(9600);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
}

void loop() {
  int red, green, blue;

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  red = pulseIn(sensorOut, LOW);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  green = pulseIn(sensorOut, LOW);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blue = pulseIn(sensorOut, LOW);

  Serial.print("R: ");
  Serial.print(red);
  Serial.print(" G: ");
  Serial.print(green);
  Serial.print(" B: ");
  Serial.println(blue);

  delay(500);
}
