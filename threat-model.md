The main asset we want to protect in AirDraw is the integrity of the gesture commands and the user's control over the connected computer.
Our system uses an ESP32-S3 and an MPU-6050 motion sensor to recognize hand gestures and translate them into computer actions, such as 
navigating presentation slides or controlling media playback.
A realistic threat is a nearby person who attempts to send unauthorized commands or use the controller without permission,
potentially disrupting a presentation or causing unintended computer actions. To reduce this risk,we propose accepting only predefined gesture commands,
requiring the physical push button to activate gesture recognition, using a confidence threshold to reject uncertain predictions,
and restricting wireless communication to the authorized paired computer if Bluetooth is implemented. 
We will also avoid storing unnecessary personal information or credentials in the public GitHub repository.
These protections are proportionate because AirDraw is a low-risk educational prototype with limited functionality, 
and they reduce realistic security risks without introducing unnecessary complexity or cost.
