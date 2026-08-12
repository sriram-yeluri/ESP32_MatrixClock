# ESP32 Matrix Clock Professional

A modular ESP32 based MAX7219 LED Matrix clock.

Designed for Arduino IDE with a clean service-based architecture.

---

## Features

- ESP32 WiFi support
- NTP synchronized clock
- Date display
- Day display
- Scrolling messages
- MAX7219 FC16 matrix support
- Modular firmware architecture
- Future Web UI ready

---

## Hardware

### Controller

ESP32 DevKit


### Display

MAX7219 FC16 LED Matrix

Recommended:

16 x 8x8 modules


### Wiring

| MAX7219 | ESP32 |
|---|---|
| VCC | 5V |
| GND | GND |
| DIN | GPIO23 |
| CLK | GPIO18 |
| CS | GPIO5 |


---

## Arduino Libraries Required

Install from Arduino Library Manager:

### MD_Parola

### MD_MAX72XX


---

## Project Structure
