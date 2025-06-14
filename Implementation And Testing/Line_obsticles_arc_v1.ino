#include <Servo.h>

// Motor control pins
#define IN1 8
#define IN2 9
#define ENA 3
#define IN3 10
#define IN4 11
#define ENB 5

// Line sensors
const int L_S = 2;
const int R_S = 4;

// Ultrasonic sensor
#define trigPin 13
#define echoPin 12

// Servo
Servo servo;
#define SERVO_PIN A5

// Speed settings
const int speedL = 65;
const int speedR = 55;

// Obstacle threshold (in cm)
const int Set = 45;

void setup() {
  pinMode(L_S, INPUT);
  pinMode(R_S, INPUT);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo.attach(SERVO_PIN);
  servo.write(90);

  Serial.begin(9600);
}

void loop() {
  int leftSensor = digitalRead(L_S);
  int rightSensor = digitalRead(R_S);
  int distance_F = Ultrasonic_read();

  if ((rightSensor == 0) && (leftSensor == 0)) {
    if (distance_F > Set) {
      forward();
    } else {
      Stop();
      avoidObstacle();
    }
  } else if ((rightSensor == 1) && (leftSensor == 0)) {
    turnRight();
  } else if ((rightSensor == 0) && (leftSensor == 1)) {
    turnLeft();
  } else {
    Stop();
  }
}

void avoidObstacle() {
  // Scan left
  servo.write(150);
  delay(400);
  int leftDist = Ultrasonic_read();

  // Scan right
  servo.write(30);
  delay(400);
  int rightDist = Ultrasonic_read();

  // Center servo
  servo.write(90);
  delay(200);

  // Decide direction
  bool avoidLeft;
  if (leftDist == 999 && rightDist == 999) avoidLeft = true;
  else if (leftDist == 999) avoidLeft = false;
  else if (rightDist == 999) avoidLeft = true;
  else avoidLeft = (leftDist > rightDist + 10);

  // Backup
  backward();
  delay(300);
  Stop();
  delay(100);

  if (avoidLeft) {
    dynamicArcTurn(70, 110, true);  // Left boosted
    Stop();
    delay(200);
    turnRight();
    delay(350);
    Stop();
    delay(100);
  } else {
    dynamicArcTurn(110, 70, false); // Right boosted
    Stop();
    delay(200);
    turnLeft();
    delay(350);
    Stop();
    delay(100);
  }

  // Try to rejoin the line slowly
  rejoinLine();
}

// Rejoin line logic: slow scan for line up to 3 seconds
void rejoinLine() {
  const unsigned long timeout = 3000;
  unsigned long startTime = millis();

  while (millis() - startTime < timeout) {
    int leftSensor = digitalRead(L_S);
    int rightSensor = digitalRead(R_S);

    if ((leftSensor == 0) && (rightSensor == 0)) {
      forward();  // line found
      return;
    } else if ((leftSensor == 1) && (rightSensor == 0)) {
      slowTurnLeft();
    } else if ((rightSensor == 1) && (leftSensor == 0)) {
      slowTurnRight();
    } else {
      slowForward();
    }
    delay(150);
  }

  // If timeout reached, just stop
  Stop();
}

// Smooth arc turn with minimum PWM threshold
void dynamicArcTurn(int leftTarget, int rightTarget, bool isLeftArc) {
  const int steps = 10;
  const int minPWM = 60;

  for (int i = 1; i <= steps; i++) {
    int leftPWM = max(minPWM, map(i, 1, steps, minPWM, leftTarget));
    int rightPWM = max(minPWM, map(i, 1, steps, minPWM, rightTarget));

    analogWrite(ENA, leftPWM);
    analogWrite(ENB, rightPWM);

    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);

    delay(50);
  }
}

// Distance reading
int Ultrasonic_read() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  return (duration == 0) ? 999 : duration / 29 / 2;
}

// Standard movement
void forward() {
  analogWrite(ENA, speedL); analogWrite(ENB, speedR);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void backward() {
  analogWrite(ENA, speedL); analogWrite(ENB, speedR);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnRight() {
  analogWrite(ENA, speedL); analogWrite(ENB, speedR);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnLeft() {
  analogWrite(ENA, speedL); analogWrite(ENB, speedR);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void Stop() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// Slower versions for scanning
void slowForward() {
  analogWrite(ENA, 50); analogWrite(ENB, 45);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void slowTurnLeft() {
  analogWrite(ENA, 50); analogWrite(ENB, 45);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void slowTurnRight() {
  analogWrite(ENA, 50); analogWrite(ENB, 45);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
