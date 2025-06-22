#include <Servo.h>

Servo myServo;
const int servoPin = 11;

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  myServo.write(0);
  Serial.println("Servo at 0°");
  delay(1000);

  myServo.write(90);
  Serial.println("Servo at 90°");
  delay(1000);

  myServo.write(180);
  Serial.println("Servo at 180°");
  delay(1000);
}
