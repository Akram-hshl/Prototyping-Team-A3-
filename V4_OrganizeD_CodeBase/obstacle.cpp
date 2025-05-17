#include <Arduino.h>
#include "config.h"
#include "sensor.h"
#include "servo_control.h"
#include "motor.h"

void handleObstacle() {
  int distance_Left = 0;
  int distance_Right = 0;

  for (int angle = 70; angle >= 0; angle -= 5) {
    servoPulse(servo, angle);
  }
  delay(500);
  distance_Left = Ultrasonic_read();

  if (distance_Left > OBSTACLE_THRESHOLD) {
    turnLeft(); delay(2000); Stop(); delay(200);
    for (int angle = 0; angle <= 70; angle += 5) servoPulse(servo, angle);
    delay(300);
    forward(); delay(2000); Stop(); delay(200);
    for (int angle = 70; angle <= 140; angle += 5) servoPulse(servo, angle);
    delay(300);
    turnRight(); delay(2000); Stop(); delay(200);
    for (int angle = 140; angle >= 70; angle -= 5) servoPulse(servo, angle);
    delay(300);
  } else {
    for (int angle = 70; angle <= 140; angle += 5) servoPulse(servo, angle);
    delay(500);
    distance_Right = Ultrasonic_read();

    if (distance_Right > OBSTACLE_THRESHOLD) {
      turnRight(); delay(2000); Stop(); delay(200);
      for (int angle = 140; angle >= 70; angle -= 5) servoPulse(servo, angle);
      delay(300);
      forward(); delay(2000); Stop(); delay(200);
      turnLeft(); delay(2000); Stop(); delay(200);
    } else {
      for (int angle = 140; angle >= 70; angle -= 5) servoPulse(servo, angle);
      delay(300);
      backward(); delay(2000); Stop(); delay(200);
      turnLeft(); delay(2000); Stop(); delay(200);
      forward(); delay(4000); Stop(); delay(200);
      turnRight(); delay(2000); Stop(); delay(200);
    }
  }
}
