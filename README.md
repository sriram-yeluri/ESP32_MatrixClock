
Core Features
✅ NTP Time Synchronization
✅ Wi-Fi Auto Reconnect
✅ Large Digital Clock
✅ Seconds Display
✅ Date
✅ Day of Week
✅ Scrolling Messages
✅ Multiple Message Pages
✅ Page Manager
✅ Custom Fonts
✅ Status Icons
✅ Logging
✅ Version Information

## Flow 

Network
   │
   ├── WiFi Connection
   ├── WiFi Reconnect
   └── configTime()
            │
            ▼
        ESP32 SNTP
            │
            ▼
         Clock Class
            │
      Formatted Strings
            │
            ▼
         Display Class


## Architecture

                ESP32_MatrixClock.ino

                         |
                         |
                    Settings
                         |
        --------------------------------
        |              |              |
     Network        Display         Clock
        |
        |
      NTP


        |
        |
 MessageManager ---- PageManager


                    ESP32_MatrixClock.ino
                             │
        ┌────────────────────┼──────────────────┐
        │                    │                  │
        ▼                    ▼                  ▼
    Network              Clock              Display
        │                    │                  │
        │                    │                  │
        ├── WiFi             │                  │
        ├── NTP              │                  │
        ├── Reconnect        │                  │
        └── configTime()     │                  │
                             │
                             └── getLocalTime()



Core
 ├── Config
 ├── Types
 ├── Constants
 └── Version

Services
 ├── Network
 ├── Clock
 ├── MessageManager
 └── Display

Controllers
 └── PageManager

Application
 └── ESP32_MatrixClock.ino


## folder structure

ESP32_MatrixClock/
│
├── ESP32_MatrixClock.ino
│
├── src/
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