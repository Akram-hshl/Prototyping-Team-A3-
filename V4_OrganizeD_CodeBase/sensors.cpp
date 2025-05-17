
#include <Arduino.h>
#include "config.h"
#include "motor.h"

long Ultrasonic_read() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn(echo, HIGH, 30000);
  if (time == 0) return 400;
  return time / 29 / 2;
}

void lineFollow() {
  int left = digitalRead(L_S);
  int right = digitalRead(R_S);
  int state = (left << 1) | right;

  switch (state) {
    case 0b00: forward(); break;
    case 0b01: turnRight(); break;
    case 0b10: turnLeft(); break;
    case 0b11: Stop(); break;
  }
}
