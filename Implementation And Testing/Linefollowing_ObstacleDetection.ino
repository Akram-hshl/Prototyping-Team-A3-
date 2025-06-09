// Motor control pins for L298N
#define IN1 8
#define IN2 9
#define ENA 3

#define IN3 10
#define IN4 11
#define ENB 5

// Line sensor pins
const int L_S = 2;
const int R_S = 4;

// Ultrasonic sensor pins
const int trigPin = 13;
const int echoPin = 12;

// Speed settings
const int speedL = 55;
const int speedR = 55;

// Obstacle detection
const int Set = 25;  

int distance_F = 50;

void setup() {
  pinMode(L_S, INPUT);
  pinMode(R_S, INPUT);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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
      avoidObstacle();
      rejoinLine();
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

void avoidObstacle() {
  Stop(); 
  delay(100);

  backward();
  Stop(); 
  //delay(600);

  turnRight(); 
  delay(600);

  // Move forward until obstacle is cleared
  while (Ultrasonic_read() <= Set) {
    forward();
    delay(50);
    Stop();
    delay(50);
    turnLeft();
    delay(50);
  }
  
  Stop();
  delay(200);
}

void rejoinLine() {
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
}

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

// Movement functions
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
