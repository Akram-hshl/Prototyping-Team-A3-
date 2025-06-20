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

// Speed settings
const int RespeedL = 50;
const int RespeedR = 50;
const int speedL = 55;
const int speedR = 55;
const int speedLa = 75;
const int speedRa = 75;

// Obstacle detection
const int Set = 35;  

int distance_F = 0;

void setup() {
  pinMode(L_S, INPUT);
  pinMode(R_S, INPUT);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(servo, OUTPUT);

  // Initialize servo to face forward
  servoPulse(servo, 70);
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
  Stop();
  delay(300);
  aturnRight();
  delay(400);
  Stop();
  delay(400);
  aforward();
  delay(500);
  Stop();
  delay(200);
  aturnLeft();
  delay(400);
  Stop();
  delay(200);
  Reforward();
  delay(400);
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
// Servo pulse function
void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(20);
}
// Movement functions
void forward() {
  analogWrite(ENA, speedL); analogWrite(ENB, speedR);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void Reforward() {
  analogWrite(ENA, RespeedL); analogWrite(ENB, RespeedR);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void aforward() {
  analogWrite(ENA, speedLa); analogWrite(ENB, speedRa);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}
void backward() {
  analogWrite(ENA, speedLa); analogWrite(ENB, speedRa);
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