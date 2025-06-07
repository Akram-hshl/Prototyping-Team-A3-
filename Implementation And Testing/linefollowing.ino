// Motor control pins for L298N
#define IN1 8     // Left motor IN1
#define IN2 9     // Left motor IN2
#define ENA 3     // Left motor speed (PWM)

#define IN3 10    // Right motor IN3
#define IN4 11    // Right motor IN4
#define ENB 5     // Right motor speed (PWM)

// Sensor pins
int L_S = 2;
int R_S = 4;

// Speed settings (0 - 255)
int speedL = 120;
int speedR = 120;

void setup() { 
  pinMode(R_S, INPUT); 
  pinMode(L_S, INPUT); 
  pinMode(ENA, OUTPUT); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  delay(1000);
}

void loop() {  
  int leftSensor = digitalRead(L_S);
  int rightSensor = digitalRead(R_S);

  // print sensor status
  Serial.print("Left Sensor: ");
  Serial.print(leftSensor);
  Serial.print(" | Right Sensor: ");
  Serial.println(rightSensor);

  if (rightSensor == 0 && leftSensor == 0) {
    forward(); // both sensors on white
  } else if (rightSensor == 1 && leftSensor == 0) {
    turnRight(); // right sensor on black
  } else if (rightSensor == 0 && leftSensor == 1) {
    turnLeft(); // left sensor on black
  } else if (rightSensor == 1 && leftSensor == 1) {
    Stop(); // both sensors on black
  }
}

void forward() {
  analogWrite(ENA, speedL);
  analogWrite(ENB, speedR);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, speedL);
  analogWrite(ENB, speedR);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  analogWrite(ENA, speedL);
  analogWrite(ENB, speedR);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Stop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

