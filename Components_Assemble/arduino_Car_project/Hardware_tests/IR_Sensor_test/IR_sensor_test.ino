const int irLeft = A0;
const int irRight = A1;

void setup() {
  Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
}

void loop() {
  int leftVal = digitalRead(irLeft);
  int rightVal = digitalRead(irRight);
  
  Serial.print("IR Left: ");
  Serial.print(leftVal);
  Serial.print(" | IR Right: ");
  Serial.println(rightVal);
  
  delay(300);
}
