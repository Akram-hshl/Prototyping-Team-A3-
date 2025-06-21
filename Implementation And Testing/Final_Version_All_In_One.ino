// Motor control pins for L298N
#define IN1 8
#define IN2 9
#define ENA 3

#define IN3 10
#define IN4 11
#define ENB 5
#define servo A5 // Servo motor control pin

// Line sensor pins
const int L_S = 2;
const int R_S = 4;

// Ultrasonic sensor pins
const int trigPin = 13;
const int echoPin = 12;
#define S0_PIN A4
#define S1_PIN A3
#define S2_PIN A2
#define S3_PIN A1
#define OUT_PIN A0

// Optional if you want to control LED and OE via code
//#define OE_PIN  7
//#define LED_PIN 6


// Speed settings
const int speedL = 55;
const int speedR = 50;
const int speedLa = 75;
const int speedRa = 70;
const int speedLp = 120;
const int speedRp = 120;

// Obstacle detection
const int Set = 5;  

int distance_F = 0;

void setup() {
  pinMode(L_S, INPUT);
  pinMode(R_S, INPUT);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(servo, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(OUT_PIN, INPUT);
  digitalWrite(S0_PIN, HIGH);
  digitalWrite(S1_PIN, LOW);

  // Initialize servo to face forward
  servoPulse(servo, 70);
  digitalWrite(S0_PIN, HIGH);
  digitalWrite(S1_PIN, LOW);
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  distance_F = Ultrasonic_read();
  Serial.print("Distance Forward = ");
  Serial.print(distance_F);
  Serial.println(" cm");

  int leftSensor = digitalRead(L_S);
  int rightSensor = digitalRead(R_S);

  if ((rightSensor == 0) && (leftSensor == 0)) {
    if (distance_F > Set) {
      forward();
    } else {
      Stop();
      delay(1000);
      int color = getColor();
      if (color == 1) {
        ParkCar(); //red = 1 park
      } else if (color == 2) {
        avoidObstacle();
        // 2 = blue aboidobstacle
      } else if (color == 3) {
        push(); // Green = 3 push
      }
      //avoidObstacle();
      //rejoinLine();
    }
  } else if ((rightSensor == 1) && (leftSensor == 0)) {
    turnRight();
  } else if ((rightSensor == 0) && (leftSensor == 1)) {
    turnLeft();
  } else if ((rightSensor == 1) && (leftSensor == 1)) {

    Stop();
  }

  delay(10);
}
void avoidObstacle(){
  int distance_Left = 0;
  int distance_Right = 0;
  Stop();
  delay(500);

  for (int angle = 70; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  delay(500);
  distance_Right = Ultrasonic_read();
  Serial.print("Right Distance: ");
  Serial.println(distance_Right);
  delay(100);
  if (distance_Right > Set){
  // Turn right 90°
  aturnRight();
  delay(300); //changed from 800
  Stop();
  delay(1000); //changed from 200
  for (int angle = 0; angle <= 70; angle += 5) {
        servoPulse(servo, angle);
      }
       // Move forward (1st side of box)
  aforward();
  delay(800); //need to adjust
  Stop();
  delay(1000); //changed from 200

  // Turn left 90°
  aturnLeft();
  delay(300); //changed from 600
  Stop();
  delay(300); //changed from 200
  
}else {
    for (int angle = 70; angle <= 140; angle += 5) {
        servoPulse(servo, angle);
      }
      delay(500);
      distance_Left = Ultrasonic_read();
      Serial.print("Left Distance: ");
      Serial.println(distance_Left);
      delay(100);

      if (distance_Left > Set) { 
  aturnLeft();
  delay(300);
  Stop();
  delay(1000);
  for (int angle = 140; angle >= 70; angle -= 5) {
            servoPulse(servo, angle);
          }
     // Move forward (1st side of box)
  aforward();
  delay(600); //need to adjust
  Stop();
  delay(1000); //changed from 200

  // Turn Right 90°
  aturnRight();
  delay(400);
  Stop();
  delay(300);
}
}
}

/*void rejoinLine() {
  unsigned long start = millis();
  const unsigned long timeout = 5000;

  while (millis() - start < timeout) {
    int leftSensor = digitalRead(L_S);
    int rightSensor = digitalRead(R_S);

    if (leftSensor == 0 && rightSensor == 0) {
      forward();
      return;
    } else if (leftSensor == 1 && rightSensor == 0) {
      turnLeft();
    } else if (rightSensor == 1 && leftSensor == 0) {
      turnRight();
    } else if (leftSensor == 1 && rightSensor == 1) {
      forward();
    } else {
      // Mild left-right scan
      turnLeft(); 
      delay(200);
      turnRight(); 
      delay(200);
    }
    delay(50);
  }
  Stop();
}*/

int Ultrasonic_read() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;

  return duration / 29 / 2;
}
// Servo pulse function
void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(20);
}
int getColor() {
  int redValue, greenValue, blueValue;

  // Red
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, LOW);
  redValue = pulseIn(OUT_PIN, LOW);

  // Green
  digitalWrite(S2_PIN, HIGH);
  digitalWrite(S3_PIN, HIGH);
  greenValue = pulseIn(OUT_PIN, LOW);

  // Blue
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, HIGH);
  blueValue = pulseIn(OUT_PIN, LOW);

  Serial.print("Red: ");
  Serial.print(redValue);
  Serial.print("\tGreen: ");
  Serial.print(greenValue);
  Serial.print("\tBlue: ");
  Serial.println(blueValue);

  // Identify lowest value as dominant color
  if (redValue < greenValue && redValue < blueValue) {
    return 1; // Red
  } else if (blueValue < redValue && blueValue < greenValue) {
    return 2; // Blue
  } else if (greenValue < redValue && greenValue < blueValue) {
    return 3; // Green
  } else {
    return 0; // Unknown or balanced
  }
}
// Movement functions
void forward() {
  analogWrite(ENA, speedL); analogWrite(ENB, speedR);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void aforward() {
  analogWrite(ENA, speedLa); analogWrite(ENB, speedRa);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void pforward() {
  analogWrite(ENA, speedLp); analogWrite(ENB, speedRp);
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
void aturnRight() {
  analogWrite(ENA, speedLa); analogWrite(ENB, speedRa);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnLeft() {
  analogWrite(ENA, speedL); analogWrite(ENB, speedR);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}
void aturnLeft() {
  analogWrite(ENA, speedLa); analogWrite(ENB, speedRa);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void Stop() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
void push(){
  // 2 blue aboidobsticles
  pforward();
  delay(500); //need to adjust
  Stop();
  delay(500);
  
}
void ParkCar() {
  // 3 Green push
  aturnRight();
  delay(300); //changed from 800
  Stop();
  delay(1000);
  backward();
  delay(5000);
   
}
