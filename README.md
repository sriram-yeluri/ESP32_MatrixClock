# ESP32 Matrix Clock Professional

A modular ESP32 firmware for a MAX7219 LED matrix clock with NTP synchronisation, rotating display pages, scrolling messages, and a browser-based message editor.

---

## Features

- NTP-synchronised clock (24-hour or 12-hour)
- Three-zone display layout: scrolling content · seconds · time
- Rotating pages: Time, Date, Day of Week, Custom Messages
- Up to 10 custom scrolling messages with configurable duration
- Priority (pinned) messages that display before regular messages
- Browser-based message editor served over Wi-Fi
- Automatic Wi-Fi reconnection
- AP fallback mode when station connection fails
- Brightness control (0–15)
- Persistent message storage via ESP32 NVS (Preferences)

---

## Hardware

### Controller

ESP32 DevKit (any variant with SPI support)

### Display

MAX7219 FC16 8×8 LED matrix modules — 16 modules recommended for the full three-zone layout.

### Wiring

| MAX7219 Pin | ESP32 GPIO |
|-------------|------------|
| VCC         | 5 V        |
| GND         | GND        |
| DIN         | GPIO 23    |
| CLK         | GPIO 18    |
| CS / LOAD   | GPIO 5     |

Pin assignments are defined in `Config.h` (`Config::Matrix`) and can be changed there.

---

## Dependencies

Install via the Arduino Library Manager:

| Library      | Purpose                          |
|--------------|----------------------------------|
| MD_Parola    | Parola text animation engine     |
| MD_MAX72XX   | MAX7219 hardware driver          |

The following are part of the ESP32 Arduino core and require no separate installation:

- `WiFi.h`
- `WebServer.h`
- `Preferences.h`
- `time.h` / SNTP

---

## Quick Start

1. Open `ESP32_MatrixClock.ino` in Arduino IDE.
2. Edit `Config.h` — set your Wi-Fi SSID, password, and timezone offset.
3. Select your ESP32 board and COM port.
4. Upload.
5. Open the Serial Monitor at 115200 baud to see the device IP address.
6. Navigate to `http://<device-ip>/` in a browser to manage messages.

---

## Configuration Reference (`Config.h`)

```cpp
namespace Config
{
    namespace WiFi
    {
        SSID        // Wi-Fi network name
        PASSWORD    // Wi-Fi password
        HOSTNAME    // mDNS / DHCP hostname (default: ESP32-MatrixClock)
    }

    namespace Time
    {
        TimezoneOffset  // UTC offset in seconds (e.g. 19800 = UTC+5:30 for IST)
        DaylightOffset  // DST offset in seconds (0 if not used)
        NtpServer       // NTP server hostname (default: pool.ntp.org)
    }

    namespace Matrix
    {
        DataPin   // SPI MOSI — GPIO 23
        ClockPin  // SPI CLK  — GPIO 18
        CSPin     // SPI CS   — GPIO 5
        Devices   // Number of 8×8 modules (default: 16)
    }

    namespace Display
    {
        DefaultBrightness  // 0–15 (default: 3)
    }
}
```

---

## Display Layout

With 16 modules the display is split into three zones:

```
┌─────────────────────────────────────────────┐
│  Zone 0 (modules 0–11)  │ Z1 │ Zone 2 (13–15) │
│  Scrolling content      │ SS │   HH : MM      │
│  Date / Day / Message   │    │   Time         │
└─────────────────────────────────────────────┘
                            ↑
                        Seconds (zone 1, module 12)
```

**Zone 2** (rightmost 3 modules) always shows the current time in HH:MM format using a 7-segment style font.

**Zone 1** (centre module) always shows the current seconds.

**Zone 0** (left 12 modules) scrolls the content for the active page.

The 3-zone split is defined in `Config.h` and can be adjusted if you want a different balance.

With fewer than 8 modules a two-zone layout is used (no seconds zone).

---

## Page Rotation

The `PageManager` cycles through enabled pages. Each page has a configurable display duration in seconds.

| Page    | Zone 0 Content          | Default Duration |
|---------|-------------------------|------------------|
| Time    | Current date            | 10 s             |
| Date    | Current date            | 10 s             |
| Day     | Day of the week         | 10 s             |
| Message | Next custom message     | 15 s             |
| Version | Firmware version string | disabled         |
| WiFi    | Connection status       | disabled         |

Pages marked disabled are defined but not shown in the default rotation.

---

## Message Management

### Via the Web Interface

Connect to `http://<device-ip>/` on any browser on the same network.

- **Add** — type a message and set its display duration (seconds).
- **Delete** — remove a message by index.
- **Update** — edit a message text, duration, or pin status in place.
- **Pin to top** — marks a message as Priority so it always plays before regular messages.
- **Clear All** — removes every message.

Messages are stored in ESP32 NVS flash and survive power cycles.

### Defaults

If no messages are found in storage on first boot, a set of default messages is loaded automatically.

### Limits

| Limit               | Value |
|---------------------|-------|
| Maximum messages    | 10    |
| Maximum text length | 127 characters |
| Minimum duration    | 1 s   |
| Maximum duration    | 30 s  |

---

## Wi-Fi Behaviour

1. On boot the device connects to the configured SSID in station (STA) mode.
2. If the connection succeeds, NTP time is synchronised within 5 seconds.
3. If the connection fails after 8 seconds, the device starts a Wi-Fi access point:
   - **SSID:** `ESP32-MatrixClock`
   - **Password:** `matrixclock`
   - **IP address:** `192.168.4.1`
4. Once in AP mode the message web server is still accessible at `http://192.168.4.1/`.
5. In station mode, reconnection is attempted every 15 seconds if the link is lost.

---

## Architecture

```
ESP32_MatrixClock.ino   ← setup() / loop() only
        │
        ├── Settings          Holds runtime config (SSID, timezone, brightness)
        │
        ├── ClockNetwork      WiFi connection, NTP sync, AP fallback
        │
        ├── Clock             Reads local time every second, formats display strings
        │
        ├── Display           MD_Parola wrapper; owns the three-zone layout
        │
        ├── MessageManager    Stores, rotates, and persists custom messages
        │
        ├── PageManager       Advances through enabled display pages on a timer
        │
        ├── DisplayScheduler  Detects page changes and pushes content to Display
        │
        └── MessageWebServer  HTTP server for the browser message editor
```

### Module Responsibilities

| Module              | File(s)                          | Responsibility |
|---------------------|----------------------------------|----------------|
| `Settings`          | Settings.h / .cpp                | Runtime configuration values loaded from `Config.h`; placeholder for future NVS persistence |
| `ClockNetwork`      | ClockNetwork.h / .cpp            | WiFi STA connect, AP fallback, NTP `configTime()`, periodic RSSI refresh |
| `Clock`             | Clock.h / .cpp                   | Calls `getLocalTime()` once per second; exposes `getTime()`, `getDate()`, `getDay()`, `getSeconds()` |
| `Display`           | Display.h / .cpp                 | Wraps MD_Parola; manages three-zone layout; deduplicates zone writes to avoid flicker |
| `MessageManager`    | MessageManager.h / .cpp          | Fixed-size ring of `DisplayMessage` structs; priority-aware rotation; NVS read/write |
| `PageManager`       | PageManager.h / .cpp             | Timed page rotation; supports enable/disable per page |
| `DisplayScheduler`  | DisplayScheduler.h / .cpp        | Coordinates clock refresh and page-change rendering; single source of truth for what the display shows |
| `MessageWebServer`  | MessageWebServer.h / .cpp        | Lightweight HTTP server (add / delete / update / pin / clear) with HTML UI |

### Shared Types

| File          | Contents |
|---------------|----------|
| `Config.h`    | Hardware pin assignments, Wi-Fi credentials, timezone, brightness defaults |
| `Constants.h` | Buffer sizes, timing intervals, display and message limits |
| `Types.h`     | `ClockState`, `ClockStatus`, `NetworkState`, `NetworkStatus`, `PageType`, `MessageType` |
| `Version.h`   | Firmware name, version string, build timestamp |
| `Font_Data.h` | Custom 7-segment font bitmaps for the clock and seconds zones |

---

## File Structure

```
ESP32_MatrixClock/
├── ESP32_MatrixClock.ino   Main sketch — setup() and loop()
├── Config.h                Hardware and user configuration
├── Constants.h             Compile-time limits and timing constants
├── Types.h                 Shared enums and structs
├── Version.h               Firmware version
├── Font_Data.h             7-segment font data
├── Settings.h / .cpp       Runtime settings accessor
├── ClockNetwork.h / .cpp   WiFi and NTP service
├── Clock.h / .cpp          Time-keeping and formatting
├── Display.h / .cpp        MAX7219 display controller
├── MessageManager.h / .cpp Message storage and rotation
├── PageManager.h / .cpp    Display page scheduling
├── DisplayScheduler.h/.cpp Rendering coordinator
├── MessageWebServer.h/.cpp Browser-based message editor
└── library.properties      Arduino library metadata
```

---

## Serial Output

The device prints status messages at **115200 baud**:

```
ESP32 Matrix Clock Professional
Firmware: 2.0.0
Connecting to WiFi...
WiFi connected.
IP address: 192.168.1.42
NTP synchronized.
[Display] Page: Time
[Display] Content(Date): 06 Aug 2026
```

---

## Firmware Version

`2.0.0` — defined in `Version.h`.

│   ├── Clock.cpp
│   ├── Clock.h
│   ├── Display.cpp
│   ├── Display.h
│   ├── NetworkManager.cpp
│   ├── NetworkManager.h
│   ├── Settings.cpp
│   ├── Settings.h
│   ├── MessageManager.cpp
│   ├── MessageManager.h
│   ├── PageManager.cpp
│   └── PageManager.h
│
├── include/
│   ├── Config.h
│   ├── Constants.h
│   ├── Types.h
│   ├── Version.h
│   └── Fonts.h
│
├── data/                  (future web files)
│   ├── index.html
│   ├── style.css
│   └── app.js
│
├── README.md
├── .gitignore
└── library.properties


### Example main loop

```cpp
void loop()
{

    network.update();

    clock.update();

    messages.update();

    pages.update();

    display.update();


    switch(
        pages.current())
    {

        case PageType::Time:

            display.showTime(
                clock.getTime()
            );

            break;


        case PageType::Message:

            display.showMessage(
                messages.current()
            );

            break;

    }

}
```

### Future Web configuration

```json
{
 "pages":
 [
  {
   "page":"TIME",
   "enabled":true,
   "duration":10
  },

  {
   "page":"MESSAGE",
   "enabled":true,
   "duration":30
  },

  {
   "page":"IP",
   "enabled":false
  }
 ]
}
```

### Access-point fallback added
If the ESP32 cannot connect to your home Wi‑Fi, it will now start its own access point so you can still open the web interface directly.

Network name : ESP32-MatrixClock
Password : matrixclock