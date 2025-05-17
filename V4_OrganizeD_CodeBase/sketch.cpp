#include "config.h"
#include "sensor.h"
#include "servo_control.h"
#include "motor.h"
#include "obstacle.h"

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

  servoPulse(servo, 70);
  delay(500);
}

void loop() {
  int distance_F = Ultrasonic_read();
  Serial.print("Front Distance: ");
  Serial.println(distance_F);

  if (distance_F <= OBSTACLE_THRESHOLD) {
    Stop();
    delay(1000);
    handleObstacle();
  } else {
    lineFollow();
  }
}
