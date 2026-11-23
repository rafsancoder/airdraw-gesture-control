#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// I2C pins for MPU-6050
#define I2C_SDA 8
#define I2C_SCL 9

// Hardware pins matching circuit diagram
#define BTN_PIN 4  // Active-low push button (internal pull-up)
#define LED_PIN 2  // Status LED (+ 220 ohm resistor)

// 50 Hz sampling rate = 20 ms interval (matches data-plan.md)
const unsigned long SAMPLE_INTERVAL_MS = 20;
unsigned long lastSampleTime = 0;

Adafruit_MPU6050 mpu;

void scanI2C() {
  Serial.println("\n--- Scanning I2C bus ---");
  byte count = 0;
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at 0x");
      if (addr < 16) Serial.print("0");
      Serial.println(addr, HEX);
      count++;
    }
  }
  if (count == 0) {
    Serial.println("ERROR: No I2C devices found! Check SDA (GPIO 8), SCL (GPIO 9), 3.3V, and GND.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  Wire.begin(I2C_SDA, I2C_SCL);
  scanI2C();

  if (!mpu.begin(0x68, &Wire)) {
    Serial.println("Failed to find MPU6050 chip! Halting.");
    while (1) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(200);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("AirDraw Ready! Hold button (GPIO 4) to stream 50Hz CSV: accX,accY,accZ,gyroX,gyroY,gyroZ");
}

void loop() {
  bool isPressed = (digitalRead(BTN_PIN) == LOW);

  // Light LED only while button is held down
  digitalWrite(LED_PIN, isPressed ? HIGH : LOW);

  unsigned long now = millis();
  if (isPressed && (now - lastSampleTime >= SAMPLE_INTERVAL_MS)) {
    lastSampleTime = now;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.print(a.acceleration.x, 4); Serial.print(",");
    Serial.print(a.acceleration.y, 4); Serial.print(",");
    Serial.print(a.acceleration.z, 4); Serial.print(",");
    Serial.print(g.gyro.x, 4);         Serial.print(",");
    Serial.print(g.gyro.y, 4);         Serial.print(",");
    Serial.println(g.gyro.z, 4);
  }
}