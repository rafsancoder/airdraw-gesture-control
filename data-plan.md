# Data plan — AirDraw: AI Gesture-Controlled Computer

**Team:** Asikul Haque Rafsan, zohab

## Sensor

We will use an MPU-6050 accelerometer and gyroscope connected to an ESP32-S3. We plan to collect motion data at approximately 50 Hz. We will train and test using the same sensor that will be used in the final device.

## Classes

- **Idle:** No intentional gesture.
- **Left swipe:** Move the device left.
- **Right swipe:** Move the device right.
- **Circle:** Draw a circular movement in the air.

The device will recognize these gestures and send commands to a computer.

## Collection

Each team member will record 20 examples of each class, including idle.

We will record indoors, using slow and normal speeds and slightly different natural grips. We will try to keep the sensor orientation consistent.

For each person and class, we will keep 5 complete recordings aside for final testing. The other 15 recordings will be used for training and validation.

## Risk

Left swipe and right swipe may be confused when the sensor is rotated or held differently. This changes the sensor-axis readings and may make the two movements appear similar.

## Waiting for

We are waiting to confirm availability or delivery of the component we need . We will start collecting data once the hardware is available. Expected arrival.
