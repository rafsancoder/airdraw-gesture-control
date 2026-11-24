#include <AirDraw-YouTube_inferencing.h> // Use the exact #include inserted by Sketch -> Include Library
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define I2C_SDA 8
#define I2C_SCL 9
#define BTN_PIN 4
#define LED_PIN 2

Adafruit_MPU6050 mpu;
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Wire.begin(I2C_SDA, I2C_SCL);
  if (!mpu.begin(0x68, &Wire)) {
    while (1) delay(100);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  if (digitalRead(BTN_PIN) == LOW) {
    digitalWrite(LED_PIN, HIGH);

    // Record 30 samples at 50 Hz (600 ms window)
    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 6) {
      uint64_t next_tick = micros() + 20000;
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      features[ix + 0] = a.acceleration.x;
      features[ix + 1] = a.acceleration.y;
      features[ix + 2] = a.acceleration.z;
      features[ix + 3] = g.gyro.x;
      features[ix + 4] = g.gyro.y;
      features[ix + 5] = g.gyro.z;

      while (micros() < next_tick) { }
    }
    digitalWrite(LED_PIN, LOW);

    // Run Edge Impulse neural network inference on the ESP32-S3
    signal_t signal;
    numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    ei_impulse_result_t result = { 0 };

    if (run_classifier(&signal, &result, false) == EI_IMPULSE_OK) {
      float best_val = 0.0f;
      const char* best_label = "idle";

      for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (result.classification[ix].value > best_val) {
          best_val = result.classification[ix].value;
          best_label = result.classification[ix].label;
        }
      }

      if (best_val >= 0.65f && strcmp(best_label, "idle") != 0) {
        Serial.println(best_label);
      }
    }

    while (digitalRead(BTN_PIN) == LOW) delay(10);
    delay(150);
  }
}

/*code for the record gestures, use this code to flash the esp32 so recording can work
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define I2C_SDA 8
#define I2C_SCL 9
#define BTN_PIN 4
#define LED_PIN 2

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Wire.begin(I2C_SDA, I2C_SCL);
  if (!mpu.begin(0x68, &Wire)) {
    while (1) delay(100);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  if (digitalRead(BTN_PIN) == LOW) {
    digitalWrite(LED_PIN, HIGH);
    uint64_t next_tick = micros() + 20000; // 50 Hz

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.print(a.acceleration.x, 4); Serial.print(",");
    Serial.print(a.acceleration.y, 4); Serial.print(",");
    Serial.print(a.acceleration.z, 4); Serial.print(",");
    Serial.print(g.gyro.x, 4);        Serial.print(",");
    Serial.print(g.gyro.y, 4);        Serial.print(",");
    Serial.println(g.gyro.z, 4);

    while (micros() < next_tick) { }
  } else {
    digitalWrite(LED_PIN, LOW);
    delay(5);
  }
}
*/