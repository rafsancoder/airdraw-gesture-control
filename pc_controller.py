import time
import serial
import pyautogui

SERIAL_PORT = "COM6"
BAUD_RATE = 115200

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
print(f"AirDraw YouTube Controller active on {SERIAL_PORT}...")

def send_shift_key(key_char):
    pyautogui.keyDown("shift")
    time.sleep(0.05)
    pyautogui.press(key_char)
    time.sleep(0.05)
    pyautogui.keyUp("shift")

while True:
    gesture = ser.readline().decode("utf-8", errors="ignore").strip().lower()
    if not gesture:
        continue

    print(f"Detected: {gesture}")
    if gesture == "swipe_right":
        send_shift_key("n")                        # Next video in playlist/queue
    elif gesture == "swipe_left":
        send_shift_key("p")                        # Previous video in player history
    elif gesture == "seek_fwd":
        pyautogui.press("l")                       # Forward 10s
    elif gesture == "seek_back":
        pyautogui.press("j")                       # Rewind 10s
    elif gesture == "restart":
        pyautogui.press("0")                       # Restart video from 0:00
    elif gesture == "vol_up":
        pyautogui.press("volumeup", presses=3)     # Global Windows volume up
    elif gesture == "vol_down":
        pyautogui.press("volumedown", presses=3)   # Global Windows volume down