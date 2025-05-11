// Motor pins
#define enA 10  // Enable pin for Left Motor
#define in1 9   // Left Motor Input 1
#define in2 8   // Left Motor Input 2
#define in3 7   // Right Motor Input 1
#define in4 6   // Right Motor Input 2
#define enB 5   // Enable pin for Right Motor

// Sensors and Servo
#define L_S A0  // Left IR sensor
#define R_S A1  // Right IR sensor
#define echo A2 // Ultrasonic echo pin
#define trigger A3 // Ultrasonic trigger pin
#define servo A5 // Servo motor control pin

int Set = 10; // Obstacle detection threshold (in cm)

void setup() {
  Serial.begin(9600);

  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(servo, OUTPUT);

  // Initialize servo to face forward
  servoPulse(servo, 70);
  delay(500);
}

void loop() {
  int distance_F = Ultrasonic_read();
  Serial.print("Front Distance: ");
  Serial.println(distance_F);

  if (distance_F <= Set) {
    Stop();
    delay(1000);
    handleObstacle();
  } else {
    lineFollow();
  }
}

// --- Functions ---

void lineFollow() {
  int left = digitalRead(L_S);
  int right = digitalRead(R_S);

  int state = (left << 1) | right;

  switch (state) {
    case 0b00: forward(); break;   // Both on line
    case 0b01: turnRight(); break; // Right off
    case 0b10: turnLeft(); break;  // Left off
    case 0b11: Stop(); break;      // Both off
  }
}

void handleObstacle() {
  int distance_Left = 0;
  int distance_Right = 0;

  // Check left
  for (int angle = 70; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  delay(500);
  distance_Left = Ultrasonic_read();
  Serial.print("Left Distance: ");
  Serial.println(distance_Left);
  delay(100);

  switch (distance_Left > Set) {
    case true:
      // Left is clear
      turnLeft(); delay(2000); Stop(); delay(200);
      for (int angle = 0; angle <= 70; angle += 5) {
        servoPulse(servo, angle);
      }
      delay(300);
      forward(); delay(2000); Stop(); delay(200);
      for (int angle = 70; angle <= 140; angle += 5) {
        servoPulse(servo, angle);
      }
      delay(300);
      turnRight(); delay(2000); Stop(); delay(200);
      for (int angle = 140; angle >= 70; angle -= 5) {
        servoPulse(servo, angle);
      }
      delay(300);
      break;

    case false:
      // Left blocked, check right
      for (int angle = 70; angle <= 140; angle += 5) {
        servoPulse(servo, angle);
      }
      delay(500);
      distance_Right = Ultrasonic_read();
      Serial.print("Right Distance: ");
      Serial.println(distance_Right);
      delay(100);

      switch (distance_Right > Set) {
        case true:
          // Right is clear
          turnRight(); delay(2000); Stop(); delay(200);
          for (int angle = 140; angle >= 70; angle -= 5) {
            servoPulse(servo, angle);
          }
          delay(300);
          forward(); delay(2000); Stop(); delay(200);
          turnLeft(); delay(2000); Stop(); delay(200);
          break;

        case false:
          // Both sides blocked
          for (int angle = 140; angle >= 70; angle -= 5) {
            servoPulse(servo, angle);
          }
          delay(300);

          backward(); delay(2000); Stop(); delay(200);
          turnLeft(); delay(2000); Stop(); delay(200);
          forward(); delay(4000); Stop(); delay(200);
          turnRight(); delay(2000); Stop(); delay(200);
          break;
      }
      break;
  }
}

// Servo pulse function
void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(20);
}

// Ultrasonic reading
long Ultrasonic_read() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn(echo, HIGH, 30000);
  if (time == 0) return 400;
  return time / 29 / 2;
}

// Motor control
void forward() {
  analogWrite(enA, 255); analogWrite(enB, 255);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void backward() {
  analogWrite(enA, 255); analogWrite(enB, 255);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
}

void turnRight() {
  analogWrite(enA, 255); analogWrite(enB, 255);
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void turnLeft() {
  analogWrite(enA, 255); analogWrite(enB, 255);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}

void Stop() {
  analogWrite(enA, 0); analogWrite(enB, 0);
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}
