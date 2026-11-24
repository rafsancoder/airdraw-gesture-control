import os
import time
import serial

PORT = "COM6"
BAUD = 115200
OUT_DIR = "gesture_dataset"
os.makedirs(OUT_DIR, exist_ok=True)

ser = serial.Serial(PORT, BAUD, timeout=0.1)
time.sleep(1.5)
ser.reset_input_buffer()

print("Connected to ESP32-S3 on", PORT)
while True:
    label = input("\nEnter gesture label (swipe_right, swipe_left, seek_fwd, seek_back, restart, vol_up, vol_down, idle) or 'q' to quit: ").strip()
    if label.lower() == "q":
        break
    if not label:
        continue

    count = int(input(f"How many samples to record for '{label}'? (e.g. 25): "))
    existing = len([f for f in os.listdir(OUT_DIR) if f.startswith(label + ".")])

    for i in range(1, count + 1):
        idx = existing + i
        print(f"[{i}/{count}] Hold button on GPIO 4 and perform '{label}' now...")
        rows = []
        ser.reset_input_buffer()

        while not rows:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            parts = line.split(",")
            if len(parts) == 6 and parts[0] != "accX":
                rows.append(parts)

        while len(rows) < 75:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            if not line:
                if len(rows) >= 20:
                    break
                continue
            parts = line.split(",")
            if len(parts) == 6 and parts[0] != "accX":
                rows.append(parts)

        filename = os.path.join(OUT_DIR, f"{label}.{idx}.csv")
        with open(filename, "w", newline="") as f:
            f.write("timestamp,accX,accY,accZ,gyroX,gyroY,gyroZ\n")
            for step, r in enumerate(rows):
                f.write(f"{step * 20},{','.join(r)}\n")

        print(f"  Saved {filename} ({len(rows)} samples)")
        time.sleep(0.4)