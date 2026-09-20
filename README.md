# Task 4.2D: Calling a Function from the Web

## Overview
This project implements a remote lighting management system for Linda's assisted living facility using an Arduino Nano 33 IoT. It allows caregivers to remotely toggle lights across three separate zones ("Living Room", "Bathroom", and "Closet") via an interactive web interface.

## System Architecture & Data Flow
- **Client Layer:** Standalone HTML5 and JavaScript interface allowing users to trigger room-specific actions.
- **Embedded Web Server:** Microcontroller acts as an HTTP server on port 80 utilizing `WiFiNINA`.
- **Core Function:** Evaluates incoming HTTP GET queries and calls `toggleLight(String room)` to execute the corresponding digital write operations.
- **Actuator Hardware:** Three independently driven LEDs on pins D2, D3, and D4 with 220Ω current-limiting resistors.

## Setup Instructions
1. Wire the LEDs to pins D2 (Living Room), D3 (Bathroom), and D4 (Closet) with current-limiting resistors.
2. Upload `Task_4.2D.ino` to the Arduino Nano 33 IoT via the Arduino IDE.
3. Obtain the assigned local IP address from the Serial Monitor.
4. Update `ARDUINO_IP` in `index.html` with the device's IP and open the file in a modern browser.
