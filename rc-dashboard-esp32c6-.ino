#include <Arduino.h>

// Pin input PWM receiver
const int PIN_STEERING = 4;   // CH1
const int PIN_THROTTLE = 5;   // CH2

// Nilai PWM standar receiver RC
const int PWM_MIN = 1000;
const int PWM_CENTER = 1500;
const int PWM_MAX = 2000;

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int readPWM(int pin) {
  uint32_t pulse = pulseIn(pin, HIGH, 30000); // timeout 30 ms
  if (pulse < 900 || pulse > 2100) {
    return PWM_CENTER;
  }
  return (int)pulse;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_STEERING, INPUT);
  pinMode(PIN_THROTTLE, INPUT);

  Serial.println("RC Dashboard Starting...");
}

void loop() {
  int steeringPWM = readPWM(PIN_STEERING);
  int throttlePWM = readPWM(PIN_THROTTLE);

  // Steering: -100% (kiri) sampai +100% (kanan)
  float steeringPct = mapFloat(steeringPWM, PWM_MIN, PWM_MAX, -100, 100);

  // Throttle: -100% (rem/mundur) sampai +100% (maju)
  float throttlePct = mapFloat(throttlePWM, PWM_MIN, PWM_MAX, -100, 100);

  // Estimasi kecepatan berdasarkan throttle maju
  float speedKmh = 0;
  if (throttlePct > 0) {
    speedKmh = throttlePct;
  }

  // Simulasi tegangan baterai
  float battery = 8.2;

  // Output ke Serial (nanti bisa diganti tampilan LCD)
  Serial.print("Steering: ");
  Serial.print(steeringPct, 1);
  Serial.print("% | Throttle: ");
  Serial.print(throttlePct, 1);
  Serial.print("% | Speed: ");
  Serial.print(speedKmh, 1);
  Serial.print(" km/h | Battery: ");
  Serial.print(battery, 2);
  Serial.println(" V");

  delay(100);
}